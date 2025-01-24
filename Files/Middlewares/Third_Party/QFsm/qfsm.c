/**********************************
 * @Description  : 有限状态机源文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-11 10:21:59
 * @FilePath     : qfsm.c
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "qfsm.h"

QEvent const QEP_reservedEvt_[4] = {
    { (QSignal)QEP_EMPTY_SIG, (uint8_t)0 },
    { (QSignal)Q_ENTRY_SIG, (uint8_t)0 },
    { (QSignal)Q_EXIT_SIG, (uint8_t)0 },
    { (QSignal)Q_INIT_SIG, (uint8_t)0 }
};

void  QFsm_init(QFsm *me, QEvent const *e)
{
    (*me->state)(me, e);
    (void)(*me->state)(me, &(QEP_reservedEvt_[Q_ENTRY_SIG]));
}

void  QFsm_dispatch(QFsm *me, QEvent const *e)
{
    QStateHandler s = me->state;

    QState r = (*s)(me, e);

    if (r == Q_RET_TRAN) {
        (void)(*s)(me, &QEP_reservedEvt_[Q_EXIT_SIG]);
        (void)(*me->state)(me, &QEP_reservedEvt_[Q_ENTRY_SIG]);
    }
}
