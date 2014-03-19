
#include <stdio.h>
#include <sys/time.h>

#include <glib.h>
#include <Ivy/ivy.h>
#include <Ivy/ivyglibloop.h>

double t1, t2;

void on_FLOW_AP_OTF(IvyClientPtr app, void *user_data, int argc, char *argv[])
{
  struct timeval t;
  
  gettimeofday(&t, NULL);
  t1 = ((double)t.tv_sec + (double)(t.tv_usec * 1e-6));
  if (t1-t2 < 0.008 || t1-t2 > 0.012)
    printf("%f\n", t1-t2);
  t2 = t1;
}

int main ( int argc, char** argv) {

  GMainLoop *ml =  g_main_loop_new(NULL, FALSE);

  //849.370 201 WIND_AP_OTF 461 0 0 0 0 31
  IvyInit ("IvyFlowApOtf", "IvyFlowApOtf READY", NULL, NULL, NULL, NULL);
  IvyBindMsg(on_FLOW_AP_OTF, NULL, "^\\S* FLOW_AP_OTF (\\S*) (\\S*) (\\S*) (\\S*) (\\S*) (\\S*)");
  IvyStart("127.255.255.255");

  g_main_loop_run(ml);

  return 0;
}

