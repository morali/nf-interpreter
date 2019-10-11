/*
 * Created on Mon Oct 07 2019
 *
 * Copyright (c) 2019 Global Control 5 Sp. z o.o.
 */

#include "isma_log_native.h"
#include <corlib_native.h>

typedef struct {
  uint64_t timestamp;
  const char *channel;
  logLevel_t level;
  char *message;
} nativeLog_t;

typedef struct {
  CLR_RT_HeapBlock *logEntry;
  CLR_RT_ProtectFromGC *gc;
} managedLog_t;

struct logEntry {
  uint32_t logId;
  uint8_t logType; // 0 - managed , 1 - nativeLog
  union {
    managedLog_t managedLog;
    nativeLog_t nativeLog;
  };
  struct logEntry *next;
};

typedef struct logEntry logEntry_t;

static logEntry_t *logListTail = NULL;
static logEntry_t *logListHead = NULL;
static uint32_t logLength = 0;
static uint32_t logId = 0;
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

    if (lt->logType == 0) {
      // remove GC protection
      delete lt->managedLog.gc;
    } else {
      // free memory allocated by message
      free(lt->nativeLog.message);
    }

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

  // set log type to managed
  newLog->logType = 0;
  // set pointer to the new logEntry
  newLog->managedLog.logEntry = logEntry;
  // protect logEntry against GarbageCollector
  newLog->managedLog.gc = new CLR_RT_ProtectFromGC(*logEntry);
  newLog->next = NULL;

  // if protection faild free memory and return
  if (newLog->managedLog.gc == NULL) {
    free(newLog);
    return false;
  }

  // asign logId and increment it
  newLog->logId = logId++;

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

extern "C" bool addLog(const char *channel, logLevel_t level, const char *message);
bool addLog(const char *channel, logLevel_t level, const char *message) {

  // allocate memory for new logEntry
  logEntry_t *newLog = (logEntry_t *)malloc(sizeof(logEntry_t));

  // return if allocation faild
  if (newLog == NULL) {
    return false;
  }

  // set logType to native
  newLog->logType = 1;

  // set channel name
  newLog->nativeLog.channel = channel;

  // calc message length
  size_t messageLen = hal_strlen_s(message) + 1;

  // allocate space for message
  newLog->nativeLog.message = (char *)malloc(messageLen);

  // if allocation failed
  if (newLog->nativeLog.message == NULL) {
    // free space for element
    free(newLog);
    return false;
  }

  // copy message
  hal_strcpy_s(newLog->nativeLog.message, messageLen, message);

  // set log level
  newLog->nativeLog.level = level;

  // set timestamp
  newLog->nativeLog.timestamp = HAL_Time_CurrentDateTime(false);

  // asign logId and increment it
  newLog->logId = logId++;

  newLog->next = NULL;

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
  logLevel_t logLevel;
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
extern "C" void addChannel(const char *channelName, logLevel_t level);
void addChannel(const char *channelName, logLevel_t level) {

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
  // set pointer to next channel to NULL
  newChannel->next = NULL;

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

  // get logEntry
  CLR_RT_HeapBlock *logEntry = stack.Arg0().Dereference();

  // get channel name and log level
  const char *channelName = logEntry[Library_isma_log_native_iSMA_Log_LogEntry::FIELD___logChannel].DereferenceString()->StringText();
  uint32_t logLevel = logEntry[Library_isma_log_native_iSMA_Log_LogEntry::FIELD___logLevel].NumericByRefConst().u4;

  // find channel with given name
  logChannel_t *channel = findChannel(channelName);

  // if channel exists and loglevel is grater then chanelLevel then add logEntry to buffer
  if (channel != NULL) {
    if (logLevel >= channel->logLevel) {
      appendLogEntry(logEntry);
    }
  }

  NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_isma_log_native_iSMA_Log_Log::GetLogs___STATIC__SZARRAY_iSMALogLogEntry__U4__U4__BYREF_U4(CLR_RT_StackFrame &stack) {
  NANOCLR_HEADER();

  CLR_RT_TypeDef_Index logEntryTypeDef;
  CLR_RT_HeapBlock &top = stack.PushValueAndClear();
  CLR_RT_HeapBlock *logEntry;
  logEntry_t *log = logListTail;

  uint32_t startIndex = stack.Arg0().NumericByRef().u4;
  uint32_t maxItemsToRetrieve = stack.Arg1().NumericByRef().u4;

  uint32_t logsNo = 0;

  logEntry_t *startLog = NULL;

  // count number of logs to return
  for (uint32_t i = 0; i < logLength; i++) {
    if (log->logId >= startIndex) {
      startLog = log;
      logsNo = logLength - i;
      break;
    }
    log = log->next;
  }

  // if number of logs exceed maxItemsToRetive then decrease number of returned logs
  if (logsNo > maxItemsToRetrieve) {
    logsNo = maxItemsToRetrieve;
  }

  // find <LogEntry> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("LogEntry", "iSMA.Log", logEntryTypeDef);
  // create an array of <LogEntry>
  NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance(top, logsNo, logEntryTypeDef));

  // get a pointer to the first object in the array (which is of type <LogEntry>)
  logEntry = (CLR_RT_HeapBlock *)top.DereferenceArray()->GetFirstElement();

  for (uint32_t i = 0; i < logsNo; i++) {
    if (startLog->logType == 0) {
      // set reference to LogEntry object
      logEntry->SetObjectReference(startLog->managedLog.logEntry);
    } else {
      // its native log
      // create an instance of <LogEntry>
      NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(*logEntry, logEntryTypeDef));

      // dereference the object in order to reach its fields
      CLR_RT_HeapBlock *hbObj = logEntry->Dereference();

      // get a reference to the timestamp managed field...
      CLR_RT_HeapBlock &timestampFieldRef = hbObj[Library_isma_log_native_iSMA_Log_LogEntry::FIELD___timestamp];
      CLR_INT64 *pRes = (CLR_INT64 *)&timestampFieldRef.NumericByRef().s8;
      // ...and set it with the timestamp
      *pRes = startLog->nativeLog.timestamp;

      // log channel name
      NANOCLR_CHECK_HRESULT(
          CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_log_native_iSMA_Log_LogEntry::FIELD___logChannel], startLog->nativeLog.channel));

      // get a reference to the logLevel managed field...
      CLR_RT_HeapBlock &logLevelFieldRef = hbObj[Library_isma_log_native_iSMA_Log_LogEntry::FIELD___logLevel];
      CLR_UINT8 *pU8Res = (CLR_UINT8 *)&logLevelFieldRef.NumericByRef().u8;
      // ...and set it with the level
      *pU8Res = startLog->nativeLog.level;

      // log text
      NANOCLR_CHECK_HRESULT(
          CLR_RT_HeapBlock_String::CreateInstance(hbObj[Library_isma_log_native_iSMA_Log_LogEntry::FIELD___text], startLog->nativeLog.message));
    }

    // set id of the last logEntry
    stack.Arg2().Dereference()->NumericByRef().u4 = startLog->logId;

    // get the next log and next logEntry in array
    startLog = startLog->next;
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
  logLevel_t logLevel = (logLevel_t)stack.Arg1().NumericByRef().u1;

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
  CLR_RT_HeapBlock &top = stack.PushValueAndClear();
  CLR_RT_HeapBlock *logChannel;
  logChannel_t *channel = logChannelTail;

  // find <DictionaryEntry> type, don't bother checking the result as it exists for sure
  g_CLR_RT_TypeSystem.FindTypeDef("DictionaryEntry", "System.Collections", dictionaryEntryTypeDef);

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

    // create an instance of <UInt8>
    NANOCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex(hbByteObj, g_CLR_RT_WellKnownTypes.m_UInt8));

    hbByteObj.NumericByRef().u1 = channel->logLevel;

    logChannel++;
    channel = channel->next;
  }

  NANOCLR_NOCLEANUP();
}