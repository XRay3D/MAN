#include "interface.h"

MI::MI()
{
    if (!semafore.available()) {
        m_man = new Man();
        m_man->moveToThread(&thread);
        m_scpi = new SCPI();
        m_scpi->moveToThread(&threadScpi);
        thread.start(QThread::NormalPriority);
        threadScpi.start(QThread::NormalPriority);
    }
    semafore.release();
}

MI::~MI()
{
    semafore.acquire();
    if (!semafore.available()) {
        thread.quit();
        thread.wait();
        threadScpi.quit();
        threadScpi.wait();
    }
}

Man* MI::man() { return m_man; }

SCPI* MI::scpi() { return m_scpi; }
