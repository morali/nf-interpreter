/*
 * Created on Mon Oct 07 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_log_native.h"
#include <corlib_native.h>

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

/**
 * @brief  Remove the oldest log.
 * @note   Run this function after add new log or after change maxLogLength
 * @retval None
 */
static void removeOldLogs() {

  // do while the list length is longer then the maximum length
  while (logLength > maxLogLength) {
    // get a pointer to tail
    logEntry_t *lt = logListTail;

    // set pointer tail pointer to the next element
    logListTail = logListTail->next;

    // remove GC protection
    delete lt->gc;

    // free space
    free(lt);

    // decrease list length
    logLength--;
  }
}

/**
 * @brief  Append new log to the log list
 * @note
 * @param  *logEntry: pointer to the new logEntry
 * @retval return false if out of memory, true on success
 */
static bool appendLogEntry(CLR_RT_HeapBlock *logEntry) {

  // allocate memory for new logEntry
  logEntry_t *newLog = (logEntry_t *)malloc(sizeof(logEntry_t));

  // return if allocation faild
  if (newLog == NULL) {
    return false;
  }

  // set pointer to the new logEntry
  newLog->logEntry = logEntry;
  // protect logEntry against GarbageCollector
  newLog->gc = new CLR_RT_ProtectFromGC(*logEntry);
  newLog->next = NULL;

  // if protection faild free memory and return
  if (newLog->gc == NULL) {
    free(newLog);
    return false;
  }

  // add new element to list
  if (logListTail == NULL) {
    logListTail = newLog;
    logListHead = newLog;
  } else {
    logListHead->next = newLog;
    logListHead = newLog;
  }

  // increase log list length
  logLength++;

  // remove oldest logs
  removeOldLogs();

  return true;
}

struct logChannel {
  char *channelName;
  uint8_t logLevel;
  struct logChannel *next;
};

typedef struct logChannel logChannel_t;

static logChannel_t *logChannelTail = NULL;
static logChannel_t *logChannelHead = NULL;
static uint32_t logChannelLength = 0;

/**
 * @brief  Find log channel with given name
 * @note
 * @param  *channelName:
 * @retval return pointer to channel or NULL if not found
 */
static logChannel_t *findChannel(const char *channelName) {

  logChannel_t *channel = logChannelTail;

  while (channel != NULL) {
    if (strcmp(channel->channelName, channelName) == 0) {
      return channel;
    } else {
      channel = channel->next;
    }
  }

  return NULL;
}

/**
 * @brief  Add new channel to list
 * @note
 * @param  *channelName: name of the new channel
 * @param  level: log level of the new channel
 * @retval None
 */
static void addChannel(const char *channelName, uint8_t level) {

  // allocate new element list
  logChannel_t *newChannel = (logChannel_t *)malloc(sizeof(logChannel_t));

  // return if allocation faild
  if (newChannel == NULL) {
    return;
  }

  // calc channelName length
  size_t nameLen = hal_strlen_s(channelName) + 1;

  // allocate space for channel name
  newChannel->channelName = (char *)malloc(nameLen);

  // if allocation failed
  if (newChannel->channelName == NULL) {
    // free space for element
    free(newChannel);
    return;
  }

  // copy name and set level
  hal_strcpy_s(newChannel->channelName, nameLen, channelName);
  newChannel->logLevel = level;

  // add element to list
  if (logChannelTail == NULL) {
    logChannelTail = newChannel;
    logChannelHead = newChannel;
  } else {
    logChannelHead->next = newChannel;
    logChannelHead = newChannel;
  }

  logChannelLength++;
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

  const char *channelName = stack.Arg0().RecoverString();
  uint8_t logLevel = stack.Arg1().NumericByRef().u1;

  // check if channel exist
  logChannel_t *channel = findChannel(channelName);

  if (channel != NULL) {
    // if exists update logLevel
    channel->logLevel = logLevel;
  } else {
    // if not exists add new channel
    addChannel(channelName, logLevel);
  }

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_log_native_iSMA_Log_Log::GetChannelSettings___STATIC__SZARRAY_mscorlibSystemCollectionsDictionaryEntry(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_TypeDef_Index dictionaryEntryTypeDef;
  CLR_RT_TypeDef_Index byteTypeDef;
  CLR_RT_HeapBlock &top = stack.PushValueAndClear();
  CLR_RT_HeapBlock *logChannel;
  logChannel_t *channel = logChannelTail;

  // find <DictionaryEntry> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("DictionaryEntry", "System.Collections", dictionaryEntryTypeDef);

  // find <Byte> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("Byte", "System", byteTypeDef);

  uint32_t len = logChannelLength;
  // create an array of <DictionaryEntry>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, len, dictionaryEntryTypeDef));

  // get a pointer to the first object in the array (which is of type <DictionaryEntry>)
  logChannel = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();
  while (len--) {

    // create an instance of <DictionaryEntry>
    NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*logChannel, dictionaryEntryTypeDef));

    // dereference the object in order to reach its fields
    CLR_RT_HeapBlock *hbObj = logChannel->Dereference();

    // channel name
    NANOCLR_CHECK_HRESULT(
        CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_corlib_native_System_Collections_DictionaryEntry::FIELD__Key], channel->channelName));

    // channel level
    CLR_RT_HeapBlock &hbByteObj = hbObj[Library_corlib_native_System_Collections_DictionaryEntry::FIELD__Value];

    // create an instance of <Byte>
    NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(hbByteObj, byteTypeDef));

    hbByteObj.NumericByRef().u1 = channel->logLevel;

    logChannel++;
    channel = channel->next;
  }

  NANOCLR_NOCLEANUP();
}