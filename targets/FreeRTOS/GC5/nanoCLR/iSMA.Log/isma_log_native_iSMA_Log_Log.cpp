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

static uint32_t maxLogLength = 1000;

static void removeOldLogs() {
  while (logLength > maxLogLength) {
    logEntry_t *lt = logListTail;

    logListTail = logListTail->next;

    delete lt->gc;
    free(lt);
    logLength--;
  }
}

static HRESULT appendLogEntry(CLR_RT_HeapBlock *logEntry) {
  NANOCLR_HEADER();

  // allocate memory for new logEntry
  logEntry_t *newLog = (logEntry_t *)malloc(sizeof(logEntry_t));

  if (newLog == NULL) {
    NANOCLR_SET_AND_LEAVE(CLR_E_OUT_OF_MEMORY);
  }
  newLog->logEntry = logEntry;
  newLog->gc = new CLR_RT_ProtectFromGC(*logEntry);
  newLog->next = NULL;

  if (newLog->gc == NULL) {
    free(newLog);
    NANOCLR_SET_AND_LEAVE(CLR_E_OUT_OF_MEMORY);
  }

  if (logListTail == NULL) {
    logListTail = newLog;
    logListHead = newLog;
  } else {
    logListHead->next = newLog;
    logListHead = newLog;
  }

  logLength++;

  removeOldLogs();
  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_log_native_iSMA_Log_Log::AddLog___STATIC__VOID__iSMALogLogEntry(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_HeapBlock *logEntry = stack.Arg0().Dereference();

  NANOCLR_CHECK_HRESULT(appendLogEntry(logEntry));

  NANOCLR_NOCLEANUP();
}

HRESULT Library_isma_log_native_iSMA_Log_Log::GetLogs___STATIC__SZARRAY_iSMALogLogEntry__U4__U4__BYREF_U4(CLR_RT_StackFrame &stack) {
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

HRESULT Library_isma_log_native_iSMA_Log_Log::SetLogBufferSize___STATIC__VOID__U2(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  maxLogLength = stack.Arg0().NumericByRef().u2;

  removeOldLogs();

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_log_native_iSMA_Log_Log::GetLogBufferSize___STATIC__U2(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_UINT16 value = maxLogLength;
  stack.SetResult_I4(value);

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_log_native_iSMA_Log_Log::SetChannelSetting___STATIC__VOID__STRING__iSMALogLogLevel(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  (void)stack;

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_log_native_iSMA_Log_Log::GetChannelSettings___STATIC__SZARRAY_mscorlibSystemCollectionsDictionaryEntry(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_TypeDef_Index dictionaryEntryTypeDef;
  CLR_RT_HeapBlock &top = stack.PushValueAndClear();

  // find <DictionaryEntry> type, don't bother checking the result as it exists for sure
  bool found = g_CLR_RT_TypeSystem.FindTypeDef("DictionaryEntry", "System.Collections", dictionaryEntryTypeDef);

  (void)found;
  // create an array of <DictionaryEntry>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, 0, dictionaryEntryTypeDef));

  NANOCLR_NOCLEANUP();
}