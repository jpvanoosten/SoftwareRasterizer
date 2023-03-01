REM The OMP_WAIT_POLICY environment variable provides a hint to an OpenMP implementation about the desired behavior of waiting threads.
REM The PASSIVE value specifies that waiting threads should mostly be passive, not consuming processor cycles, while waiting.
SETX OMP_WAIT_POLICY PASSIVE

PAUSE