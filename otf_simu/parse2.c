
#define SERIAL_PORT "/dev/ttyUSB1"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  int i, j, ser_fd;
  float a, b, c, d;
  unsigned char check;
  char output[256];
  struct termios cur_termios;

  if ((ser_fd = open(SERIAL_PORT, O_RDWR)) == 0) {
    perror("could not open serial port");
    return -1;
  }

  if (tcgetattr(ser_fd, &cur_termios)) {
    perror("getting modem serial device attr");
    return -1;
  }

  /* input modes */
  cur_termios.c_iflag &= ~(IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK|ISTRIP|INLCR|IGNCR
                           |ICRNL |IXON|IXANY|IXOFF|IMAXBEL);
    
  /* IGNCR does not pass 0x0D */
  cur_termios.c_iflag |= BRKINT;
    
  /* output_flags */
  cur_termios.c_oflag  &=~(OPOST|ONLCR|OCRNL|ONOCR|ONLRET);
    
  /* control modes */
  cur_termios.c_cflag &= ~(CSIZE|CSTOPB|CREAD|PARENB|PARODD|HUPCL|CLOCAL|CRTSCTS);
  cur_termios.c_cflag |= CREAD|CS8|CLOCAL;
    
  /* local modes */
  cur_termios.c_lflag &= ~(ISIG|ICANON|IEXTEN|ECHO|FLUSHO|PENDIN);
  cur_termios.c_lflag |= NOFLSH;
    
  if (cfsetispeed(&cur_termios, B0)) {
    perror("setting input modem serial device speed");
    return -1;
  }
    
  if (cfsetospeed(&cur_termios, B115200)) {
    perror("setting modem serial device speed");
    return -1;
  }
    
  if (tcsetattr(ser_fd, TCSADRAIN, &cur_termios)) {
    perror("setting modem serial device attr");
    return -1;
  }
  
  for (i=1; i<1000000; i++) {
                  
    a = 0;
    b = 0;
    c = 0;
    d = 0;

    sprintf(output, "%05d,%.2f,%.2f,%.2f,%04.1f,", i, a, b, c, d);
//    printf("%s", output);
    write(ser_fd, output, strlen(output));

    check = 0;
    for (j=0; j < strlen(output); j++) check ^= output[j];

    sprintf(output, "%x\r\n", check);
//    printf("%s", output);
    write(ser_fd, output, strlen(output));
    
    usleep(9880);
  }
  
  close(ser_fd);

  return 0;
}
