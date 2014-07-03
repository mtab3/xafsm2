#include <QSound>

#include "MainWindow.h"

void MainWindow::PlayEndingSound( void )
{
  PlaySound( "finished.wav", 3 );
}

void MainWindow::PlayGoOnSound( void )
{
  PlaySound( "finished.wav", 3 );
}

void MainWindow::PlaySound( QString name, int times )
{
#if 1
  QSound sound( name );
  // ������ QSound *sound = new QSound; �����
  // �Ǹ��(or �ɤ�����) delete sound; ���ڥ���ɬ�ס�
  // ���δؿ���Ǥ����Ȥ�ʤ����Τʤ顢
  // local �ѿ��ˤ��� new, delete ̵���ˤ�������������

  sound.setLoops( times );
  sound.play();
#else
  // QSound �ǤϤʤ� QtMultimedia ��ȤäƤߤ褦����...
  // ���ݤ��������ʤΤǤޤ��Ȥ��ʤ�

  inputFile.setFileName( name );
  inputFile.open(QIODevice::ReadOnly);
  
  QAudioFormat format;
  // Set up the format, eg.
  format.setFrequency(8000);
  format.setChannels(1);
  format.setSampleSize(8);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::UnSignedInt);
  
  QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
  if (!info.isFormatSupported(format)) {
    qWarning()<<"raw audio format not supported by backend, cannot play audio.";
    return;
  }
  
  audioOutput = new QAudioOutput(format, this);
  connect(audioOutput,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
  audioOutput->start(&inputFile);

#endif
}
