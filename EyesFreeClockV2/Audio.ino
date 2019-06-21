void playAudio(int track){

  audioData = audioTracks[track];
    
  audioPlaying = true;
  audioSize = audioData[0];
  audioFrameIndex = 1;
}

void stopAudio(){
  audioPlaying = false;
}

bool isPlaying(){
  return audioPlaying;
}

void processAudioFrame(){
  unsigned long currentMicros = micros();
  if(currentMicros - audioLastMicros > SAMPLERATEMICROS){
    //we are ready to play the next frame
    dac.setVoltage(pgm_read_word(&audioData[audioFrameIndex])>>4, false);
    audioFrameIndex++;
    if(audioFrameIndex < audioSize){
      audioLastMicros = currentMicros;
    } else {
      stopAudio();
    }
    
  }
}
