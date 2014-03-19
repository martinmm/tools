
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXL 256

//#define OTF "WIND_AP_OTF"
#define OTF "FLOW_AP_OTF"

int main(int argc, char *argv[]) {

  char* fname;
  char in_str[MAXL], id[MAXL];
  int nbr=0;
  float t, t_prev, t_begin, t_end;
  int ac, cnt, crs_a, crs_b, crs_c, alt, chk;
  FILE * f;
  
  if (argc == 2) fname = argv[1];
  else return -1;
  
  f = fopen(fname, "r");
  if (f == NULL) {
    perror("open");
    return -1;
  }
  
  while (!feof(f)) {
    fgets(in_str, MAXL, f);

/*
   <message name="WIND_PROBE" id="111">
     <field name="counter"    type="uint32"/>
     <field name="course_a"   type="int16"  unit="decideg" alt_unit="deg"/>
     <field name="course_b"   type="int16"  unit="decideg" alt_unit="deg"/>
     <field name="course_c"   type="int16"  unit="decideg" alt_unit="deg"/>
     <field name="altitude"   type="int32"  unit="cm" alt_unit="m"/>
     <field name="checksum"   type="uint8"/>
   </message>
*/	

    //14.080 201 FLOW_AP_OTF 76628 0 0 0 0 17
	//849.370 201 WIND_AP_OTF 461 0 0 0 0 31
    sscanf(in_str, "%f %d %s %d %d %d %d %d %d", &t, &ac, id, &cnt, &crs_a, &crs_b, &crs_c, &alt, &chk);

    if (strncmp(OTF, id, sizeof(OTF)) == 0) {
	  if (nbr++ == 0) t_begin = t;
	  if (t-t_prev > 0.015 || (t-t_prev < 0.008)) {
#if 1	  
        printf("%.3f\n", t-t_prev);
#else
        printf("%.3f\n", t_prev);
        printf("%.3f %d %s %d %d %d %d %d %d\n", t, ac, id, cnt, crs_a, crs_b, crs_c, alt, chk);
#endif
      }
	  t_prev = t;
      t_end = t;
	}
  }
  printf("nbr = %d, t_delta = %.3f s, t_average = %.3f ms\n", nbr, t_end - t_begin, ((t_end - t_begin) * 1000.) / nbr);
  return 0;
}
