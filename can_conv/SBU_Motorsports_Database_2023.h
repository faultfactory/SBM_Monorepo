/** CAN message encoder/decoder: automatically generated - do not edit
  * Generated by dbcc: See https://github.com/howerj/dbcc */
#ifndef SBU_MOTORSPORTS_DATABASE_2023_H
#define SBU_MOTORSPORTS_DATABASE_2023_H

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
	/* Body_Roll_deg: Roll angle of the vehicle body in degrees

Output is an integer to save space

Resolution of 0.1 degrees */
	/* scaling 0.1, offset 0.0, units deg  */
	uint16_t Body_Roll_deg;
	/* Body_Pitch_deg_test2: Pitch angle of the vehicle body in degrees

Output is an integer to save space

Resolution of 0.1 degrees */
	/* scaling 0.1, offset 0.0, units deg  */
	uint16_t Body_Pitch_deg_test2;
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
	dbcc_time_stamp_t can_0x100_IMU_time_stamp_rx;
	dbcc_time_stamp_t can_0x101_GPS_time_stamp_rx;
	unsigned can_0x100_IMU_status : 2;
	unsigned can_0x100_IMU_tx : 1;
	unsigned can_0x100_IMU_rx : 1;
	unsigned can_0x101_GPS_status : 2;
	unsigned can_0x101_GPS_tx : 1;
	unsigned can_0x101_GPS_rx : 1;
	can_0x100_IMU_t can_0x100_IMU;
	can_0x101_GPS_t can_0x101_GPS;
} POSTPACK can_obj_sbu_motorsports_database_2023_h_t;

int unpack_message(can_obj_sbu_motorsports_database_2023_h_t *o, const unsigned long id, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp);
int pack_message(can_obj_sbu_motorsports_database_2023_h_t *o, const unsigned long id, uint64_t *data);
int print_message(const can_obj_sbu_motorsports_database_2023_h_t *o, const unsigned long id, FILE *output);

int decode_can_0x100_Body_Roll_deg(const can_obj_sbu_motorsports_database_2023_h_t *o, double *out);
int encode_can_0x100_Body_Roll_deg(can_obj_sbu_motorsports_database_2023_h_t *o, double in);
int decode_can_0x100_Body_Pitch_deg_test2(const can_obj_sbu_motorsports_database_2023_h_t *o, double *out);
int encode_can_0x100_Body_Pitch_deg_test2(can_obj_sbu_motorsports_database_2023_h_t *o, double in);
int decode_can_0x100_Body_Yaw_deg(const can_obj_sbu_motorsports_database_2023_h_t *o, double *out);
int encode_can_0x100_Body_Yaw_deg(can_obj_sbu_motorsports_database_2023_h_t *o, double in);


int decode_can_0x101_Body_Speed_mps(const can_obj_sbu_motorsports_database_2023_h_t *o, double *out);
int encode_can_0x101_Body_Speed_mps(can_obj_sbu_motorsports_database_2023_h_t *o, double in);


#ifdef __cplusplus
} 
#endif

#endif
