/*
 * Created on Mon Oct 07 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_log_native.h"

struct logEntry {
  CLR_RT_HeapBlock *logEntry;
  CLR_RT_ProtectFromGC *gc;
  struct logEntry *next;
};

typedef struct logEntry logEntry_t;

static logEntry_t *logListTail = NULL;
static logEntry_t *logListHead = NULL;
static uint32_t logLength = 0;

static uint32_t maxLogLength = 10;

static void removeOldLog() {
  logEntry_t *lt = logListTail;

  logListTail = logListTail->next;

  delete lt->gc;
  platform_free(lt);
  logLength--;
}

static void appendLogEntry(CLR_RT_HeapBlock *logEntry) {
  logEntry_t *newLog = (logEntry_t *)platform_malloc(sizeof(logEntry_t));
  newLog->logEntry = logEntry;
  newLog->gc = new CLR_RT_ProtectFromGC(*logEntry);
  newLog->next = NULL;

  if (logListTail == NULL) {
    logListTail = newLog;
    logListHead = newLog;
  } else {
    logListHead->next = newLog;
    logListHead = newLog;
  }

  logLength++;

  while (logLength > maxLogLength) {
    removeOldLog();
  }
}

HRESULT Library_isma_log_native_iSMA_Log_Log::AddLog___STATIC__VOID__iSMALogLogEntry(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_HeapBlock *logEntry = stack.Arg0().Dereference();

  appendLogEntry(logEntry);

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_log_native_iSMA_Log_Log::GetLogs___STATIC__SZARRAY_iSMALogLogEntry(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_TypeDef_Index logEntryTypeDef;
  CLR_RT_HeapBlock &top = stack.PushValueAndClear();
  CLR_RT_HeapBlock *logEntry;
  logEntry_t *log = logListTail;

  // find <LogEntry> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("LogEntry", "iSMA.Log", logEntryTypeDef);

  uint32_t logsNo = logLength;
  // create an array of <LogEntry>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, logsNo, logEntryTypeDef));

  // get a pointer to the first object in the array (which is of type <LogEntry>)
  logEntry = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

  for (uint32_t i = 0; i < logsNo; i++) {
    // set reference to LogEntry object
    logEntry->SetObjectReference(log->logEntry);

    log = log->next;
    logEntry++;
  }

  NANOCLR_NOCLEANUP();
}