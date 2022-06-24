# Simple C Calculator

## Overview

Calculator that gets string with polynomial expression and calculates it.
order.

## Dependencies

[CUnit](http://cunit.sourceforge.net/) library is required for testing.

## Usage

### Build and run

Build with
```
$ make build
```
Run with
```
$ ./AlikCalc "1+2*3"
```

### Install
Can be installed to /bin with
```
$ make
$ make install
```

### Expression format

Infix format without spaces. Braces supported.  
Supported operators:  
*, /, +, - (not unary)

### Tests

To run CUnit tests:
```
$ make test
$ ./Test
```