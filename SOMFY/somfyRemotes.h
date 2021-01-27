

struct somfyRemote
{
  int number;
  int id;
};

struct somfyRemote remote1;
struct somfyRemote remote2;
struct somfyRemote remote3;
struct somfyRemote remote4;
struct somfyRemote remote5;
struct somfyRemote remote6;
struct somfyRemote remote7;
struct somfyRemote remote8;
struct somfyRemote remote9;

void initializeRemotes()
{
  int startId = 0x140807;
  int startNr = 0;

  
  startNr = 0x01;
  remote1.number = startNr;
  remote1.id = startId + startNr;

  startNr = 0x02;
  remote2.number = startNr;
  remote2.id = startId + startNr;

  startNr = 0x03;
  remote3.number = startNr;
  remote3.id = startId + startNr;

  startNr = 0x04;
  remote4.number = startNr;
  remote4.id = startId + startNr;

  startNr = 0x05;
  remote5.number = startNr;
  remote5.id = startId + startNr;

  startNr = 0x06;
  remote6.number = startNr;
  remote6.id = startId + startNr;

  startNr = 0x07;
  remote7.number = startNr;
  remote7.id = startId + startNr;

  startNr = 0x08;
  remote8.number = startNr;
  remote8.id = startId + startNr;

  startNr = 0x09;
  remote9.number = startNr;
  remote9.id = startId + startNr;
}
