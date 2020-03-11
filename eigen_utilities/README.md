# Purpose

Provide utilities for adding debug information in Eigen.

## Testing

With the following command, you can create and test multiple build configurations. Pay attention to log files and differences in the results

    ./scripts/run_tests.sh | tee share/results.yaml
    git diff -- share/results.yaml
