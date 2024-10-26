#ifndef FIB_H
#define FIB_H

class Fib {
private:
    int a, b;

public:
    // ���캯������ʼ��쳲��������е�ǰ������
    Fib(int n = 1) : a(0), b(1) {
        while (b < n) {
            next();
        }
    }

    // ��ȡ��ǰ��쳲�������
    int get() const {
        return b;
    }

    // ��ȡ��һ��쳲�������
    void next() {
        int c = a + b;
        a = b;
        b = c;
    }

    // ��ȡǰһ��쳲�������
    void prev() {
        int c = b - a;
        b = a;
        a = c;
    }
};

#endif // FIB_H
