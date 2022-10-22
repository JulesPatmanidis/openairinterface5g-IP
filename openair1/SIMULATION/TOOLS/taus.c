/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
//#include "SIMULATION/TOOLS/sim.h"


static unsigned int s0, s1, s2;

//----------------------------------------------
//
/*!
*/
//

unsigned int taus(void)
{
  unsigned int b = (((s0 << 13) ^ s0) >> 19);
  s0 = (((s0 & 0xFFFFFFFE) << 12)^  b);
  b = (((s1 << 2) ^ s1) >> 25);
  s1 = (((s1 & 0xFFFFFFF8) << 4)^  b);
  b = (((s2 << 3) ^ s2) >> 11);
  s2 = (((s2 & 0xFFFFFFF0) << 17)^  b);
  return s0 ^ s1 ^ s2;
}

void set_taus_seed(unsigned int seed_init)
{

  struct drand48_data buffer;
  unsigned long result = 0;

  if (seed_init == 0) {
    unsigned int data[3];
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
    {
      abort();
    }
    if (read(fd, data, sizeof(data)) != sizeof(data))
    {
      abort();
    }
    close(fd);
    s0 = data[0];
    s1 = data[1];
    s2 = data[2];
  } else {
    /* Use reentrant version of rand48 to ensure that no conflicts with other generators occur */
    srand48_r((long int)seed_init, &buffer);
    mrand48_r(&buffer, (long int *)&result);
    s0 = result;
    mrand48_r(&buffer, (long int *)&result);
    s1 = result;
    mrand48_r(&buffer, (long int *)&result);
    s2 = result;
  }
}

#ifdef MAIN

main()
{
  unsigned int i,rand;

  set_taus_seed();

  for (i=0; i<10; i++) {
    rand = taus();
    printf("%u\n",rand);
  }
}
#endif //MAIN