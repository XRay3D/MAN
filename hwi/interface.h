#pragma once

#include "man.h"

#include <QSemaphore>
#include <QThread>
#include <scpi.h>

class MI {
    static inline Man* m_man = nullptr;
    static inline SCPI* m_scpi = nullptr;

    static inline QThread thread;
    static inline QThread threadScpi;
    static inline QSemaphore semafore;

public:
    MI();
    ~MI();
    static Man* man();
    static SCPI* scpi();
};
