/** CAN message encoder/decoder: automatically generated - do not edit
  * Generated by dbcc: See https://github.com/howerj/dbcc */
#ifndef SBM_NETWORK_DEFINITION_H
#define SBM_NETWORK_DEFINITION_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" { 
#endif

#ifndef PREPACK
#define PREPACK
#endif

#ifndef POSTPACK
#define POSTPACK
#endif

#ifndef DBCC_TIME_STAMP
#define DBCC_TIME_STAMP
typedef uint32_t dbcc_time_stamp_t; /* Time stamp for message; you decide on units */
#endif

#ifndef DBCC_STATUS_ENUM
#define DBCC_STATUS_ENUM
typedef enum {
	DBCC_SIG_STAT_UNINITIALIZED_E = 0, /* Message never sent/received */
	DBCC_SIG_STAT_OK_E            = 1, /* Message ok */
	DBCC_SIG_STAT_ERROR_E         = 2, /* Encode/Decode/Timestamp/Any error */
} dbcc_signal_status_e;
#endif

typedef PREPACK struct {
	uint16_t FL_Wheelspeed_rpm; /* scaling 0.1, offset 0.0, units rpm  */
	uint16_t FR_Wheelspeed_rpm; /* scaling 0.1, offset 0.0, units rpm  */
} POSTPACK can_0x098_Wheel_Speeds_t;

typedef PREPACK struct {
	int16_t FL_Press_psi; /* scaling 0.1, offset 0.0, units psi  */
	int16_t FR_Press_psi; /* scaling 0.1, offset 0.0, units psi  */
} POSTPACK can_0x099_Brake_Pressures_t;

typedef PREPACK struct {
	/* Body_Roll_deg: Roll angle of the vehicle body in degrees

Output is an integer to save space

Resolution of 0.1 degrees */
	/* scaling 0.1, offset 0.0, units deg  */
	uint16_t Body_Roll_deg;
	/* Body_Pitch_deg: Pitch angle of the vehicle body in degrees

Output is an integer to save space

Resolution of 0.1 degrees */
	/* scaling 0.1, offset 0.0, units deg  */
	uint16_t Body_Pitch_deg;
	/* Body_Yaw_deg: Yaw angle of the vehicle body in degrees

Output is an integer to save space

Resolution of 0.1 degrees */
	/* scaling 0.1, offset 0.0, units deg  */
	uint16_t Body_Yaw_deg;
} POSTPACK can_0x100_IMU_t;

typedef PREPACK struct {
	/* Body_Speed_mps: Velocity of the vehicle body in m/s

Resolution of 0.1 m/s (0.224 mph) */
	/* scaling 0.1, offset 0.0, units m/s  */
	uint8_t Body_Speed_mps;
} POSTPACK can_0x101_GPS_t;

typedef PREPACK struct {
	/* Raw_Accel_x_mps2: Raw acceleration data from the accelerometer (m/s^2)
x-Axis */
	/* scaling 0.1, offset 0.0, units m/s^2  */
	int16_t Raw_Accel_x_mps2;
	/* Raw_Accel_y_mps2: Raw acceleration data from the accelerometer (m/s^2)
y-Axis */
	/* scaling 0.1, offset 0.0, units m/s^2  */
	int16_t Raw_Accel_y_mps2;
	/* Raw_Accel_z_mps2: Raw acceleration data from the accelerometer (m/s^2)
z-Axis */
	/* scaling 0.1, offset 0.0, units m/s^2  */
	int16_t Raw_Accel_z_mps2;
	/* Raw_Gyro_x_rps: Raw angular velocity data from gyroscope (rad/s)
x-Axis */
	/* scaling 0.1, offset 0.0, units rad/s  */
	int8_t Raw_Gyro_x_rps;
	/* Raw_Gyro_y_rps: Raw angular velocity data from gyroscope (rad/s)
y-Axis */
	/* scaling 0.1, offset 0.0, units rad/s  */
	int8_t Raw_Gyro_y_rps;
	/* Raw_Gyro_z_rps: Raw angular velocity data from gyroscope (rad/s)
z-Axis */
	/* scaling 0.1, offset 0.0, units rad/s  */
	int8_t Raw_Gyro_z_rps;
} POSTPACK can_0x102_RAW_Accel_Gyro_t;

typedef PREPACK struct {
	/* Raw_Mag_x_uT: Raw magnetic field data from magnetometer (microTesla)
x-Axis */
	/* scaling 1.0, offset 0.0, units uTesla  */
	int8_t Raw_Mag_x_uT;
	/* Raw_Mag_y_uT: Raw magnetic field data from magnetometer (microTesla)
y-Axis */
	/* scaling 1.0, offset 0.0, units uTesla  */
	int8_t Raw_Mag_y_uT;
	/* Raw_Mag_z_uT: Raw magnetic field data from magnetometer (microTesla)
z-Axis */
	/* scaling 1.0, offset 0.0, units uTesla  */
	int8_t Raw_Mag_z_uT;
} POSTPACK can_0x103_RAW_Mag_t;

typedef PREPACK struct {
	uint8_t CAN_Logger; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x104_CAN_Logger_End_t;

typedef PREPACK struct {
	uint8_t CAN_Logger; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x105_CAN_Logger_Begin_t;

typedef PREPACK struct {
	dbcc_time_stamp_t can_0x098_Wheel_Speeds_time_stamp_rx;
	dbcc_time_stamp_t can_0x099_Brake_Pressures_time_stamp_rx;
	dbcc_time_stamp_t can_0x100_IMU_time_stamp_rx;
	dbcc_time_stamp_t can_0x101_GPS_time_stamp_rx;
	dbcc_time_stamp_t can_0x102_RAW_Accel_Gyro_time_stamp_rx;
	dbcc_time_stamp_t can_0x103_RAW_Mag_time_stamp_rx;
	dbcc_time_stamp_t can_0x104_CAN_Logger_End_time_stamp_rx;
	dbcc_time_stamp_t can_0x105_CAN_Logger_Begin_time_stamp_rx;
	unsigned can_0x098_Wheel_Speeds_status : 2;
	unsigned can_0x098_Wheel_Speeds_tx : 1;
	unsigned can_0x098_Wheel_Speeds_rx : 1;
	unsigned can_0x099_Brake_Pressures_status : 2;
	unsigned can_0x099_Brake_Pressures_tx : 1;
	unsigned can_0x099_Brake_Pressures_rx : 1;
	unsigned can_0x100_IMU_status : 2;
	unsigned can_0x100_IMU_tx : 1;
	unsigned can_0x100_IMU_rx : 1;
	unsigned can_0x101_GPS_status : 2;
	unsigned can_0x101_GPS_tx : 1;
	unsigned can_0x101_GPS_rx : 1;
	unsigned can_0x102_RAW_Accel_Gyro_status : 2;
	unsigned can_0x102_RAW_Accel_Gyro_tx : 1;
	unsigned can_0x102_RAW_Accel_Gyro_rx : 1;
	unsigned can_0x103_RAW_Mag_status : 2;
	unsigned can_0x103_RAW_Mag_tx : 1;
	unsigned can_0x103_RAW_Mag_rx : 1;
	unsigned can_0x104_CAN_Logger_End_status : 2;
	unsigned can_0x104_CAN_Logger_End_tx : 1;
	unsigned can_0x104_CAN_Logger_End_rx : 1;
	unsigned can_0x105_CAN_Logger_Begin_status : 2;
	unsigned can_0x105_CAN_Logger_Begin_tx : 1;
	unsigned can_0x105_CAN_Logger_Begin_rx : 1;
	can_0x098_Wheel_Speeds_t can_0x098_Wheel_Speeds;
	can_0x099_Brake_Pressures_t can_0x099_Brake_Pressures;
	can_0x100_IMU_t can_0x100_IMU;
	can_0x101_GPS_t can_0x101_GPS;
	can_0x102_RAW_Accel_Gyro_t can_0x102_RAW_Accel_Gyro;
	can_0x103_RAW_Mag_t can_0x103_RAW_Mag;
	can_0x104_CAN_Logger_End_t can_0x104_CAN_Logger_End;
	can_0x105_CAN_Logger_Begin_t can_0x105_CAN_Logger_Begin;
} POSTPACK can_obj_sbm_network_definition_h_t;

int unpack_message(can_obj_sbm_network_definition_h_t *o, const unsigned long id, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp);
int pack_message(can_obj_sbm_network_definition_h_t *o, const unsigned long id, uint64_t *data);
int print_message(const can_obj_sbm_network_definition_h_t *o, const unsigned long id, FILE *output);

int decode_can_0x098_FL_Wheelspeed_rpm(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x098_FL_Wheelspeed_rpm(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x098_FR_Wheelspeed_rpm(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x098_FR_Wheelspeed_rpm(can_obj_sbm_network_definition_h_t *o, double in);


int decode_can_0x099_FL_Press_psi(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x099_FL_Press_psi(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x099_FR_Press_psi(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x099_FR_Press_psi(can_obj_sbm_network_definition_h_t *o, double in);


int decode_can_0x100_Body_Roll_deg(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x100_Body_Roll_deg(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x100_Body_Pitch_deg(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x100_Body_Pitch_deg(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x100_Body_Yaw_deg(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x100_Body_Yaw_deg(can_obj_sbm_network_definition_h_t *o, double in);


int decode_can_0x101_Body_Speed_mps(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x101_Body_Speed_mps(can_obj_sbm_network_definition_h_t *o, double in);


int decode_can_0x102_Raw_Accel_x_mps2(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x102_Raw_Accel_x_mps2(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x102_Raw_Accel_y_mps2(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x102_Raw_Accel_y_mps2(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x102_Raw_Accel_z_mps2(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x102_Raw_Accel_z_mps2(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x102_Raw_Gyro_x_rps(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x102_Raw_Gyro_x_rps(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x102_Raw_Gyro_y_rps(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x102_Raw_Gyro_y_rps(can_obj_sbm_network_definition_h_t *o, double in);
int decode_can_0x102_Raw_Gyro_z_rps(const can_obj_sbm_network_definition_h_t *o, double *out);
int encode_can_0x102_Raw_Gyro_z_rps(can_obj_sbm_network_definition_h_t *o, double in);


int decode_can_0x103_Raw_Mag_x_uT(const can_obj_sbm_network_definition_h_t *o, int8_t *out);
int encode_can_0x103_Raw_Mag_x_uT(can_obj_sbm_network_definition_h_t *o, int8_t in);
int decode_can_0x103_Raw_Mag_y_uT(const can_obj_sbm_network_definition_h_t *o, int8_t *out);
int encode_can_0x103_Raw_Mag_y_uT(can_obj_sbm_network_definition_h_t *o, int8_t in);
int decode_can_0x103_Raw_Mag_z_uT(const can_obj_sbm_network_definition_h_t *o, int8_t *out);
int encode_can_0x103_Raw_Mag_z_uT(can_obj_sbm_network_definition_h_t *o, int8_t in);


int decode_can_0x104_CAN_Logger(const can_obj_sbm_network_definition_h_t *o, uint8_t *out);
int encode_can_0x104_CAN_Logger(can_obj_sbm_network_definition_h_t *o, uint8_t in);


int decode_can_0x105_CAN_Logger(const can_obj_sbm_network_definition_h_t *o, uint8_t *out);
int encode_can_0x105_CAN_Logger(can_obj_sbm_network_definition_h_t *o, uint8_t in);


#ifdef __cplusplus
} 
#endif

#endif
