/**********************************
 * @Description  : 有限状态机头文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-11 10:22:04
 * @FilePath     : QFsm.h
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __QFSM_H
#define __QFSM_H

#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t QSignal;

enum QReservedSignals {
    Q_ENTRY_SIG = 1,
    Q_EXIT_SIG,
    Q_INIT_SIG,
    Q_USER_SIG
};

typedef struct QEventTag {
    QSignal sig;
    uint8_t dynamic_;
} QEvent;

typedef uint8_t QState;

typedef QState (*QStateHandler)(void *me, QEvent const *e);

typedef struct QFsmTag {
    QStateHandler state;
} QFsm;

#define QFsm_ctor(me_, initial_) ((me_)->state = (initial_))

void QFsm_init(QFsm *me, QEvent const *e);
void QFsm_dispatch(QFsm *me, QEvent const *e);

#define Q_RET_HANDLED ((QState)0)
#define Q_RET_IGNORED ((QState)1)
#define Q_RET_TRAN    ((QState)2)

#define Q_HANDLED()        (Q_RET_HANDLED)
#define Q_IGNORED()        (Q_RET_IGNORED)
#define Q_TRAN(target_)    (((QFsm *)me)->state = (QStateHandler)(target_), Q_RET_TRAN)

#define QEP_EMPTY_SIG 0


#ifdef __cplusplus
}
#endif

#endif // __QFSM_H