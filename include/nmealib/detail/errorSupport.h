#pragma once

#if defined(NMEALIB_NO_EXCEPTIONS)
#define NMEALIB_RETURN_ERROR(exceptionExpr) \
    do { \
        (void)0; \
        return nullptr; \
    } while (0)

#define NMEALIB_RETURN_ERROR_VALUE(exceptionExpr, value) \
    do { \
        (void)0; \
        return (value); \
    } while (0)
#else
#define NMEALIB_RETURN_ERROR(exceptionExpr) throw (exceptionExpr)
#define NMEALIB_RETURN_ERROR_VALUE(exceptionExpr, value) throw (exceptionExpr)
#endif