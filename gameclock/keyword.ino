typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;

static inference_t inference;
static signed short sampleBuffer[2048];
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
static volatile bool record_ready = false;

bool isKeyword() {
  
  int record_sample_28;
  unsigned long mmm = millis();
  inference.buffer = (int16_t *)malloc(EI_CLASSIFIER_RAW_SAMPLE_COUNT * sizeof(int16_t));

  digitalWrite(25, HIGH);
  
  for(int i=0; i<EI_CLASSIFIER_RAW_SAMPLE_COUNT; i++){
    record_sample_28 = analogRead(28) - 512;
    if (record_sample_28 > 206) record_sample_28=206;
    if (record_sample_28 < -206) record_sample_28=-206;
    
    inference.buffer[i] = record_sample_28*159;

    delayMicroseconds(59);
  }

  digitalWrite(25, LOW);

  inference.buf_count = 0;
  inference.buf_ready = 1;
  
  

  signal_t signal;
  signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
  signal.get_data = &microphone_audio_signal_get_data;
  ei_impulse_result_t result = { 0 };

  EI_IMPULSE_ERROR r = run_classifier(&signal, &result, debug_nn);

  bool keywordCondition = 0;
  if(result.classification[2].value > result.classification[1].value){
    if(result.classification[2].value > result.classification[0].value){
      keywordCondition = 1;
    }
  }
  
  ei_free(inference.buffer);

  return keywordCondition;
}

static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);

    return 0;
}
