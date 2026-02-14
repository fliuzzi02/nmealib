# GitHub Actions Workflow Template & Best Practices

## Basic Structure of a Workflow File

A GitHub Actions workflow is a YAML file that defines automated tasks. Here's the structure:

```yaml
name: Workflow Name                    # Display name in GitHub UI

on:                                    # Trigger conditions
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]
  workflow_dispatch:                   # Manual trigger button

jobs:                                  # Collection of jobs
  job-name:                           # Unique job identifier
    runs-on: ubuntu-latest            # Runner platform

    steps:                            # Sequential steps
      - name: Step description
        run: command
```

---

## Key Concepts

### 1. **Triggers (`on:` section)**

```yaml
on:
  push:                               # Run on push
    branches: [ "main", "develop" ]
    paths: [ "src/**" ]               # Only if these paths change
  
  pull_request:                       # Run on PR
    branches: [ "main" ]
  
  schedule:                           # Run on schedule (cron syntax)
    - cron: '0 2 * * *'               # Daily at 2 AM UTC
  
  workflow_dispatch:                  # Manual trigger (button in UI)
  
  release:                            # Run on tag/release
    types: [ published ]
```

### 2. **Jobs**

Each workflow can have multiple jobs that run in **parallel** by default:

```yaml
jobs:
  job1:
    runs-on: ubuntu-latest
    # ...
  
  job2:
    runs-on: ubuntu-latest
    needs: job1                       # Wait for job1 to complete first
    # ...
```

### 3. **Runners**

Choose the environment where your job runs:

```yaml
runs-on: ubuntu-latest                # Free Linux runner
runs-on: windows-latest               # Free Windows runner
runs-on: macos-latest                 # Free macOS runner
runs-on: self-hosted                  # Your own machine
runs-on: ubuntu-22.04                 # Specific version
```

### 4. **Matrix Strategy**

Run the same job with different configurations:

```yaml
strategy:
  matrix:
    os: [ubuntu-latest, windows-latest]
    compiler: [gcc, clang]
  
  include:                            # Add custom combinations
    - os: windows-latest
      compiler: msvc
  
  exclude:                            # Remove combinations
    - os: windows-latest
      compiler: gcc
```

Access matrix values: `${{ matrix.os }}`, `${{ matrix.compiler }}`

### 5. **Steps**

Each step runs in sequence within a job:

```yaml
steps:
  - name: Checkout code
    uses: actions/checkout@v4         # Use pre-built action
  
  - name: Run command
    run: echo "Hello"                 # Run shell command
    shell: bash                       # Specify shell (bash, powershell, etc)
    working-directory: src/           # Change working directory
    env:                              # Environment variables
      MY_VAR: value
  
  - name: Conditional step
    if: github.event_name == 'push'   # Run only on push
    run: echo "Pushed"
  
  - name: Upload artifacts
    uses: actions/upload-artifact@v4
    with:
      name: my-artifact
      path: build/output/
      retention-days: 5
```

### 6. **Contexts & Variables**

Access runtime information:

```yaml
${{ github.event_name }}              # 'push', 'pull_request', etc
${{ github.ref }}                     # Branch/tag ref
${{ github.sha }}                     # Commit SHA
${{ github.actor }}                   # User who triggered
${{ github.workspace }}               # Default working directory
${{ matrix.os }}                      # Matrix variable
${{ secrets.MY_SECRET }}              # Secret from settings
${{ env.MY_VAR }}                     # Environment variable
${{ steps.my-step.outputs.my-output }} # Output from previous step
```

### 7. **Step Outputs**

Share data between steps:

```yaml
steps:
  - name: Set output
    id: my-step                       # Step ID (required for outputs)
    run: echo "version=1.2.3" >> $GITHUB_OUTPUT

  - name: Use output
    run: echo "Version is ${{ steps.my-step.outputs.version }}"
```

### 8. **Artifacts**

Upload/download files between jobs:

```yaml
# Upload (in job1)
- uses: actions/upload-artifact@v4
  with:
    name: build-artifact
    path: build/

# Download (in job2, which has needs: job1)
- uses: actions/download-artifact@v4
  with:
    name: build-artifact
```

### 9. **Caching**

Cache dependencies to speed up builds:

```yaml
- uses: actions/cache@v3
  with:
    path: ~/.cache/pip                # What to cache
    key: ${{ runner.os }}-pip-${{ hashFiles('**/requirements.txt') }}
    restore-keys: |
      ${{ runner.os }}-pip-
```

### 10. **Secrets**

Store sensitive data (tokens, passwords):

1. Go to repo → Settings → Secrets and variables → Actions
2. Click "New repository secret"
3. Use in workflow: `${{ secrets.MY_SECRET }}`

---

## Generic Job Template

```yaml
name: Generic Job Example

on:
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]

jobs:
  generic-job:
    runs-on: ${{ matrix.os }}
    
    strategy:
      fail-fast: false
      matrix:
        os: [ubuntu-latest]
        version: [16, 18, 20]
    
    steps:
      - name: Checkout code
        uses: actions/checkout@v4
      
      - name: Set up environment
        id: setup
        shell: bash
        run: |
          echo "Starting setup..."
          echo "timestamp=$(date -u +'%Y-%m-%dT%H:%M:%SZ')" >> $GITHUB_OUTPUT
      
      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y build-essential cmake
      
      - name: Build
        run: |
          mkdir -p build
          cd build
          cmake ..
          cmake --build .
      
      - name: Test
        run: |
          cd build
          ctest --output-on-failure
      
      - name: Upload results
        if: always()                  # Upload even if test fails
        uses: actions/upload-artifact@v4
        with:
          name: results-${{ matrix.os }}-v${{ matrix.version }}
          path: build/results/
      
      - name: Report
        if: success()                 # Run only if previous steps succeeded
        run: echo "✅ Job succeeded at ${{ steps.setup.outputs.timestamp }}"
```

---

## Best Practices Summary

✅ **DO:**
- Use clear, descriptive job and step names
- Fail fast during development: `fail-fast: true` (change to `false` for production)
- Use `if: always()` to upload artifacts even on failure
- Cache dependencies to reduce build time
- Separate concerns: one job = one purpose
- Use secrets for sensitive data, never commit them
- Add `--output-on-failure` to tests for better debugging
- Use matrix strategy for multi-platform support
- Document your workflows with comments

❌ **DON'T:**
- Commit secrets or tokens directly to the repo
- Use matrix for unrelated tasks (use separate jobs instead)
- Run jobs sequentially if they can run in parallel
- Ignore `if:` conditions (helps save time and costs)
- Upload large artifacts unnecessarily
- Leave workflows in failed state without investigating

---

## Common Useful Actions

```yaml
actions/checkout@v4                   # Clone repository
actions/upload-artifact@v4            # Upload files
actions/download-artifact@v4          # Download files
actions/cache@v3                      # Cache dependencies
actions/setup-python@v4               # Install Python
actions/setup-node@v3                 # Install Node.js
slackapi/slack-notify-action@v1       # Send Slack notifications
```

---

## Examples in This Repository

- **Build.yml** – Compiles on multiple platforms
- **Test.yml** – Runs tests (depends on Build)
- **StaticAnalysis.yml** – Runs code analysis (manual trigger)
- **Release.yml** – Builds release (manual or on tag)
