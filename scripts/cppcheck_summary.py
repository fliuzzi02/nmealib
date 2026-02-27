#!/usr/bin/env python3
import os
import sys
import xml.etree.ElementTree as ET
from collections import Counter, defaultdict


def _normalize_location(path: str, line: str) -> str:
    if not path:
        return "<no-location>"

    normalized_path = path
    cwd = os.getcwd()
    if os.path.isabs(path):
        try:
            rel = os.path.relpath(path, cwd)
            if not rel.startswith(".."):
                normalized_path = "/" + rel.replace("\\", "/")
        except ValueError:
            normalized_path = path.replace("\\", "/")
    else:
        normalized_path = "/" + path.replace("\\", "/")

    if line:
        return f"{normalized_path}:{line}"
    return normalized_path


def main() -> int:
    if len(sys.argv) != 2:
        print("Usage: cppcheck_summary.py <cppcheck-report.xml>", file=sys.stderr)
        return 2

    report_path = sys.argv[1]

    try:
        root = ET.parse(report_path).getroot()
    except FileNotFoundError:
        print("report_not_found: 1")
        return 0
    except ET.ParseError:
        print("invalid_xml_report: 1")
        return 0

    counts = Counter()
    locations_by_id = defaultdict(list)
    seen_locations_by_id = defaultdict(set)

    for error in root.findall('.//error'):
        issue_id = error.attrib.get('id', 'unknown')
        counts[issue_id] += 1

        location = error.find('location')
        if location is None:
            location_str = "<no-location>"
        else:
            location_str = _normalize_location(
                location.attrib.get('file', ''),
                location.attrib.get('line', ''),
            )

        if location_str not in seen_locations_by_id[issue_id]:
            seen_locations_by_id[issue_id].add(location_str)
            locations_by_id[issue_id].append(location_str)

    if not counts:
        print("no_findings: 0")
        return 0

    for issue_id, count in sorted(counts.items(), key=lambda x: (-x[1], x[0])):
        print(f"{issue_id}: {count}")
        for location in locations_by_id[issue_id][:5]:
            print(f"    - {location}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
