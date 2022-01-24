#ifndef __V_TEST_H__
#define __V_TEST_H__

#define v_test(test) \
    do { \
        printf("----------%s----------\n", #test); \
        test(); \
    } while (0)

#endif // !__V_TEST_H__