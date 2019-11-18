

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
 * @brief  Add new channel to list
 * @note
 * @param  *channelName: name of the new channel
 * @param  level: log level of the new channel
 * @retval None
 */
extern "C" {
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
}