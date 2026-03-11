# dbltoa

A small C library that converts an IEEE 754 `double` to a decimal C-string with configurable precision, optional rounding behavior, and an option to trim trailing zeros. It is built as a static library (`dbltoa.a`) and can be used standalone or as a submodule alongside `libftx`.

## Table of Contents

- [Features](#features)
- [Repository Layout](#repository-layout)
- [API Overview](#api-overview)
- [Build](#build)
- [Usage](#usage)
- [Integration](#integration)
- [Notes](#notes)

## Features

- Convert `double` to a newly allocated string (`dbltoa`, `dbltoa_precision`)
- Write into a caller-provided buffer (`dbltoa_buff`, `dbltoa_buff_prec`)
- Configurable precision up to very large values (intended to support subnormals and long expansions)
- Optional trimming of trailing zeros (via `t_dbltoa.trim_trailing_zeros`)
- Handles special values (NaN, Infinity, negative NaN bit-pattern handling is defined in the header)

## Repository layout

Typical layout:

- `include/dbltoa.h`
- `src/*.c`
- `extern_libary/libftx` (cloned by the Makefile if missing)

The library depends on `libftx` (string/memory helpers like `ft_strdup`, `ft_strlcpy`, `ft_strlen`, `ft_memset`, etc).

## API Overview

Header: `include/dbltoa.h`

Main entry points:

- `char *dbltoa(double dbl);`
  - Convenience wrapper calling `dbltoa_precision(value, 2, false)`

- `char *dbltoa_precision(double value, uint16_t prec, bool trim);`
  - Returns a heap-allocated string, caller must `free()`.
  - `prec` sets the number of digits after the decimal point (0 means no fractional part).
  - `trim` removes trailing zeros in the fractional part and also removes the decimal point if it becomes the last character (for example `1.2300` becomes `1.23`, `1.000` becomes `1`).

- `uint16_t dbltoa_buff(double value, char *buff, uint16_t b_size);`
  - Writes with a standard precision of 2 into `buff`
  - Returns number of characters written (excluding terminator), or 0 on invalid buffer size

- `uint16_t dbltoa_buff_prec(t_dbltoa dbl);`
  - Fully configurable buffered conversion via:
    - `dbl.value`
    - `dbl.buff`
    - `dbl.buff_size`
    - `dbl.precision`
    - `dbl.trim_trailing_zeros`

Types:

```c
typedef struct s_dbltoa
{
    double      value;
    char        *buff;
    uint16_t    buff_size;
    uint16_t    precision;
    bool        trim_trailing_zeros;
}   t_dbltoa;
```

## Build

### Standalone build
Build the static library:

```sh
make          # build dbltoa.a
make clean    # remove build artifacts
make fclean   # clean + remove dbltoa.a (and extern deps)
make re       # fclean + build
make valgrind # debug flags
make debug    # debug flags + sanitizers (per Makefile)
```

This repository is intended to be used as a submodule of `libftx`. The `submodule` target exists for that integration path (it adjusts include paths and avoids nested dependency setup).

```bash
make submodule
```


## Usage

### 1) Allocate a string

```c
#include <stdio.h>
#include <stdlib.h>
#include "dbltoa.h"

int main(void)
{
    char *s = dbltoa_precision(3.1415926535, 6, false);
    if (!s)
        return 1;
    printf("%s\n", s);
    free(s);
    return 0;
}
```

### 2) Write into a user buffer

```c
#include <stdio.h>
#include "dbltoa.h"

int main(void)
{
    char buf[128];

    t_dbltoa config = (t_dbltoa){
        .value = 42.5678900,
        .buff = buf,
        .buff_size = sizeof(buf),
        .precision = 6,
        .trim_trailing_zeros = true
    };

    dbltoa_buff_prec(config);
    printf("%s\n", buf);
    return 0;
}
```

## Integration

If you build `dbltoa.a` and want to link it into an executable:

```bash
cc -I include -I extern_libary/libftx/include main.c dbltoa.a -o demo
```

## Notes

- The maximum buffer sizes and precision limits are large by design (`MAX_DBL_STR_LEN`, `MAX_DBL_BUFF`, `MAX_PRECISION`).
- This project uses strict compilation flags; when debugging, sanitizers are enabled via `make debug`.
- The conversion approach is based on representing the value as a fraction (numerator and denominator strings) and performing decimal digit extraction via repeated division and subtraction, then applying precision post-processing.
