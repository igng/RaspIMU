#ifndef CONTROL_H_
#define CONTROL_H_

#define dt 1;

void control_init(void);
void control_destroy(void);
void unicycle(float *, float *);
void F_fun(void);
void ekf(void);

#endif
