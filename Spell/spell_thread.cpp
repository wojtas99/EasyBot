#include "spell_thread.h"
#include "../Functions/Game.h"
#include <QThread>


void SpellThread::run()
{
    while (m_running && !m_spells.isEmpty())
    {
        for (auto item : m_spells)
        {
            msleep(10);
        }
    }
}


