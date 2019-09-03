void loadTrack(int track){
  audioQueue[audioQueueIndex] = track;
  audioQueueIndex++;
}

void playAudio(){
  int currentTrack = audioQueue[playerIndex];
  if(currentTrack == -1){
    stopAudio();
  } else {
    audioPlaying = true;
    audioData = audioTracks[currentTrack];
    audioSize = pgm_read_word(&audioData[0]);
    audioFrameIndex = 1;
  }
}

void stopAudio(){
  audioPlaying = false;
  //reset the audio queue.
  for(int i = 0; i<8; i++){
    audioQueue[i] = -1;
  }
  audioQueueIndex = 0;
  playerIndex = 0;
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
      //play the next frame
      audioLastMicros = currentMicros;
    } else {
      if(++playerIndex != -1){
        playAudio();
      } else {
        stopAudio();
      }
      
    } 
    
  }
}
