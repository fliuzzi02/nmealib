#!/usr/bin/env python3
import sys
import xml.etree.ElementTree as ET
from collections import Counter


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
    for error in root.findall('.//error'):
        counts[error.attrib.get('id', 'unknown')] += 1

    if not counts:
        print("no_findings: 0")
        return 0

    for issue_id, count in sorted(counts.items(), key=lambda x: (-x[1], x[0])):
        print(f"{issue_id}: {count}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
