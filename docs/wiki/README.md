# Wiki Documentation

This directory contains the wiki documentation for nmealib. These markdown files are intended to be used to populate the GitHub wiki for this repository.

## Files

- **Home.md** - Wiki home page with overview and quick links
- **Getting-Started.md** - Installation and first steps guide
- **API-Reference.md** - Complete API documentation
- **NMEA-0183-Guide.md** - Detailed guide to NMEA 0183 format
- **Examples.md** - Practical code examples and use cases
- **Building-and-Testing.md** - Build instructions and testing guide
- **Contributing.md** - Contribution guidelines

## How to Use

### Option 1: Populate GitHub Wiki

1. Navigate to the repository's wiki on GitHub
2. Create new pages with the same names as these files (without the .md extension)
3. Copy the content from each markdown file into the corresponding wiki page
4. Save each page

### Option 2: Reference from Main Documentation

These files can also be linked from the main README.md or used as standalone documentation in the repository.

### Option 3: Generate Static Site

These markdown files can be used with static site generators like:
- Jekyll
- MkDocs
- Docusaurus
- Hugo

## Updating Documentation

When making changes to the library:

1. Update the relevant wiki pages in this directory
2. Ensure all code examples are tested and working
3. Update version numbers if applicable
4. Keep cross-references between pages consistent

## Documentation Standards

- Use clear, concise language
- Include code examples for all features
- Keep examples practical and runnable
- Update documentation alongside code changes
- Test all code snippets before committing

## Links Between Pages

Use relative links in the format:
```markdown
[Link Text](Page-Name.md)
```

For example:
```markdown
See the [API Reference](API-Reference.md) for details.
```

## Building the Wiki

If you want to preview these pages locally, you can use a markdown viewer or a static site generator:

```bash
# Using Python markdown viewer
pip install grip
grip Home.md

# Using MkDocs
pip install mkdocs
mkdocs serve
```

## Contributing to Documentation

Documentation contributions are welcome! Please follow the same process as code contributions:

1. Make changes to the markdown files
2. Test all code examples
3. Submit a pull request
4. Documentation changes will be reviewed like code changes

## See Also

- [Main README](../../README.md) - Project overview and quick start
- [GitHub Wiki](https://github.com/fliuzzi02/nmealib/wiki) - Published wiki pages
