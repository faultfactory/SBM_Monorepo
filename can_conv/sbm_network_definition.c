/* Generated by DBCC, see <https://github.com/howerj/dbcc> */
#include "sbm_network_definition.h"
#include <inttypes.h>
#include <assert.h>

#define UNUSED(X) ((void)(X))

static inline uint64_t reverse_byte_order(uint64_t x) {
	x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
	x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
	x = (x & 0x00FF00FF00FF00FF) << 8  | (x & 0xFF00FF00FF00FF00) >> 8;
	return x;
}

static inline int print_helper(int r, int print_return_value) {
	return ((r >= 0) && (print_return_value >= 0)) ? r + print_return_value : -1;
}

static int pack_can_0x100_IMU(can_obj_sbm_network_definition_h_t *o, uint64_t *data) {
	assert(o);
	assert(data);
	register uint64_t x;
	register uint64_t i = 0;
	/* Body_Roll_deg: start-bit 0, length 12, endianess intel, scaling 0.1, offset 0 */
	x = ((uint16_t)(o->can_0x100_IMU.Body_Roll_deg)) & 0xfff;
	i |= x;
	/* Body_Pitch_deg: start-bit 16, length 12, endianess intel, scaling 0.1, offset 0 */
	x = ((uint16_t)(o->can_0x100_IMU.Body_Pitch_deg)) & 0xfff;
	x <<= 16; 
	i |= x;
	/* Body_Yaw_deg: start-bit 32, length 12, endianess intel, scaling 0.1, offset 0 */
	x = ((uint16_t)(o->can_0x100_IMU.Body_Yaw_deg)) & 0xfff;
	x <<= 32; 
	i |= x;
	*data = (i);
	o->can_0x100_IMU_tx = 1;
	return 0;
}

static int unpack_can_0x100_IMU(can_obj_sbm_network_definition_h_t *o, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp) {
	assert(o);
	assert(dlc <= 8);
	register uint64_t x;
	register uint64_t i = (data);
	if (dlc < 8)
		return -1;
	/* Body_Roll_deg: start-bit 0, length 12, endianess intel, scaling 0.1, offset 0 */
	x = i & 0xfff;
	o->can_0x100_IMU.Body_Roll_deg = x;
	/* Body_Pitch_deg: start-bit 16, length 12, endianess intel, scaling 0.1, offset 0 */
	x = (i >> 16) & 0xfff;
	o->can_0x100_IMU.Body_Pitch_deg = x;
	/* Body_Yaw_deg: start-bit 32, length 12, endianess intel, scaling 0.1, offset 0 */
	x = (i >> 32) & 0xfff;
	o->can_0x100_IMU.Body_Yaw_deg = x;
	o->can_0x100_IMU_rx = 1;
	o->can_0x100_IMU_time_stamp_rx = time_stamp;
	return 0;
}

int decode_can_0x100_Body_Roll_deg(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x100_IMU.Body_Roll_deg);
	rval *= 0.1;
	if (rval <= 409.5) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x100_Body_Roll_deg(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x100_IMU.Body_Roll_deg = 0;
	if (in > 409.5)
		return -1;
	in *= 10;
	o->can_0x100_IMU.Body_Roll_deg = in;
	return 0;
}

int decode_can_0x100_Body_Pitch_deg(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x100_IMU.Body_Pitch_deg);
	rval *= 0.1;
	if (rval <= 409.5) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x100_Body_Pitch_deg(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x100_IMU.Body_Pitch_deg = 0;
	if (in > 409.5)
		return -1;
	in *= 10;
	o->can_0x100_IMU.Body_Pitch_deg = in;
	return 0;
}

int decode_can_0x100_Body_Yaw_deg(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x100_IMU.Body_Yaw_deg);
	rval *= 0.1;
	if (rval <= 409.5) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x100_Body_Yaw_deg(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x100_IMU.Body_Yaw_deg = 0;
	if (in > 409.5)
		return -1;
	in *= 10;
	o->can_0x100_IMU.Body_Yaw_deg = in;
	return 0;
}

int print_can_0x100_IMU(const can_obj_sbm_network_definition_h_t *o, FILE *output) {
	assert(o);
	assert(output);
	int r = 0;
	r = print_helper(r, fprintf(output, "Body_Roll_deg = (wire: %.0f)\n", (double)(o->can_0x100_IMU.Body_Roll_deg)));
	r = print_helper(r, fprintf(output, "Body_Pitch_deg = (wire: %.0f)\n", (double)(o->can_0x100_IMU.Body_Pitch_deg)));
	r = print_helper(r, fprintf(output, "Body_Yaw_deg = (wire: %.0f)\n", (double)(o->can_0x100_IMU.Body_Yaw_deg)));
	return r;
}

static int pack_can_0x101_GPS(can_obj_sbm_network_definition_h_t *o, uint64_t *data) {
	assert(o);
	assert(data);
	register uint64_t x;
	register uint64_t i = 0;
	/* Body_Speed_mps: start-bit 0, length 8, endianess intel, scaling 0.1, offset 0 */
	x = ((uint8_t)(o->can_0x101_GPS.Body_Speed_mps)) & 0xff;
	i |= x;
	*data = (i);
	o->can_0x101_GPS_tx = 1;
	return 0;
}

static int unpack_can_0x101_GPS(can_obj_sbm_network_definition_h_t *o, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp) {
	assert(o);
	assert(dlc <= 8);
	register uint64_t x;
	register uint64_t i = (data);
	if (dlc < 8)
		return -1;
	/* Body_Speed_mps: start-bit 0, length 8, endianess intel, scaling 0.1, offset 0 */
	x = i & 0xff;
	o->can_0x101_GPS.Body_Speed_mps = x;
	o->can_0x101_GPS_rx = 1;
	o->can_0x101_GPS_time_stamp_rx = time_stamp;
	return 0;
}

int decode_can_0x101_Body_Speed_mps(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x101_GPS.Body_Speed_mps);
	rval *= 0.1;
	if (rval <= 25.5) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x101_Body_Speed_mps(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x101_GPS.Body_Speed_mps = 0;
	if (in > 25.5)
		return -1;
	in *= 10;
	o->can_0x101_GPS.Body_Speed_mps = in;
	return 0;
}

int print_can_0x101_GPS(const can_obj_sbm_network_definition_h_t *o, FILE *output) {
	assert(o);
	assert(output);
	int r = 0;
	r = print_helper(r, fprintf(output, "Body_Speed_mps = (wire: %.0f)\n", (double)(o->can_0x101_GPS.Body_Speed_mps)));
	return r;
}

static int pack_can_0x102_RAW_Accel_Gyro(can_obj_sbm_network_definition_h_t *o, uint64_t *data) {
	assert(o);
	assert(data);
	register uint64_t x;
	register uint64_t i = 0;
	/* Raw_Accel_x_mps2: start-bit 0, length 9, endianess intel, scaling 0.1, offset 0 */
	x = ((uint16_t)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_x_mps2)) & 0x1ff;
	i |= x;
	/* Raw_Accel_y_mps2: start-bit 9, length 9, endianess intel, scaling 0.1, offset 0 */
	x = ((uint16_t)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_y_mps2)) & 0x1ff;
	x <<= 9; 
	i |= x;
	/* Raw_Accel_z_mps2: start-bit 18, length 9, endianess intel, scaling 0.1, offset 0 */
	x = ((uint16_t)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_z_mps2)) & 0x1ff;
	x <<= 18; 
	i |= x;
	/* Raw_Gyro_x_rps: start-bit 32, length 8, endianess intel, scaling 0.1, offset 0 */
	x = ((uint8_t)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_x_rps)) & 0xff;
	x <<= 32; 
	i |= x;
	/* Raw_Gyro_y_rps: start-bit 40, length 8, endianess intel, scaling 0.1, offset 0 */
	x = ((uint8_t)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_y_rps)) & 0xff;
	x <<= 40; 
	i |= x;
	/* Raw_Gyro_z_rps: start-bit 48, length 8, endianess intel, scaling 0.1, offset 0 */
	x = ((uint8_t)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_z_rps)) & 0xff;
	x <<= 48; 
	i |= x;
	*data = (i);
	o->can_0x102_RAW_Accel_Gyro_tx = 1;
	return 0;
}

static int unpack_can_0x102_RAW_Accel_Gyro(can_obj_sbm_network_definition_h_t *o, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp) {
	assert(o);
	assert(dlc <= 8);
	register uint64_t x;
	register uint64_t i = (data);
	if (dlc < 8)
		return -1;
	/* Raw_Accel_x_mps2: start-bit 0, length 9, endianess intel, scaling 0.1, offset 0 */
	x = i & 0x1ff;
	x = (x & 0x100) ? (x | 0xfe00) : x; 
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_x_mps2 = x;
	/* Raw_Accel_y_mps2: start-bit 9, length 9, endianess intel, scaling 0.1, offset 0 */
	x = (i >> 9) & 0x1ff;
	x = (x & 0x100) ? (x | 0xfe00) : x; 
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_y_mps2 = x;
	/* Raw_Accel_z_mps2: start-bit 18, length 9, endianess intel, scaling 0.1, offset 0 */
	x = (i >> 18) & 0x1ff;
	x = (x & 0x100) ? (x | 0xfe00) : x; 
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_z_mps2 = x;
	/* Raw_Gyro_x_rps: start-bit 32, length 8, endianess intel, scaling 0.1, offset 0 */
	x = (i >> 32) & 0xff;
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_x_rps = x;
	/* Raw_Gyro_y_rps: start-bit 40, length 8, endianess intel, scaling 0.1, offset 0 */
	x = (i >> 40) & 0xff;
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_y_rps = x;
	/* Raw_Gyro_z_rps: start-bit 48, length 8, endianess intel, scaling 0.1, offset 0 */
	x = (i >> 48) & 0xff;
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_z_rps = x;
	o->can_0x102_RAW_Accel_Gyro_rx = 1;
	o->can_0x102_RAW_Accel_Gyro_time_stamp_rx = time_stamp;
	return 0;
}

int decode_can_0x102_Raw_Accel_x_mps2(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_x_mps2);
	rval *= 0.1;
	if ((rval >= -25.6) && (rval <= 25.5)) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x102_Raw_Accel_x_mps2(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_x_mps2 = 0;
	if (in < -25.6)
		return -1;
	if (in > 25.5)
		return -1;
	in *= 10;
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_x_mps2 = in;
	return 0;
}

int decode_can_0x102_Raw_Accel_y_mps2(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_y_mps2);
	rval *= 0.1;
	if ((rval >= -25.6) && (rval <= 25.5)) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x102_Raw_Accel_y_mps2(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_y_mps2 = 0;
	if (in < -25.6)
		return -1;
	if (in > 25.5)
		return -1;
	in *= 10;
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_y_mps2 = in;
	return 0;
}

int decode_can_0x102_Raw_Accel_z_mps2(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_z_mps2);
	rval *= 0.1;
	if ((rval >= -25.6) && (rval <= 25.5)) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x102_Raw_Accel_z_mps2(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_z_mps2 = 0;
	if (in < -25.6)
		return -1;
	if (in > 25.5)
		return -1;
	in *= 10;
	o->can_0x102_RAW_Accel_Gyro.Raw_Accel_z_mps2 = in;
	return 0;
}

int decode_can_0x102_Raw_Gyro_x_rps(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_x_rps);
	rval *= 0.1;
	if ((rval >= -12.8) && (rval <= 12.7)) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x102_Raw_Gyro_x_rps(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_x_rps = 0;
	if (in < -12.8)
		return -1;
	if (in > 12.7)
		return -1;
	in *= 10;
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_x_rps = in;
	return 0;
}

int decode_can_0x102_Raw_Gyro_y_rps(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_y_rps);
	rval *= 0.1;
	if ((rval >= -12.8) && (rval <= 12.7)) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x102_Raw_Gyro_y_rps(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_y_rps = 0;
	if (in < -12.8)
		return -1;
	if (in > 12.7)
		return -1;
	in *= 10;
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_y_rps = in;
	return 0;
}

int decode_can_0x102_Raw_Gyro_z_rps(const can_obj_sbm_network_definition_h_t *o, double *out) {
	assert(o);
	assert(out);
	double rval = (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_z_rps);
	rval *= 0.1;
	if ((rval >= -12.8) && (rval <= 12.7)) {
		*out = rval;
		return 0;
	} else {
		*out = (double)0;
		return -1;
	}
}

int encode_can_0x102_Raw_Gyro_z_rps(can_obj_sbm_network_definition_h_t *o, double in) {
	assert(o);
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_z_rps = 0;
	if (in < -12.8)
		return -1;
	if (in > 12.7)
		return -1;
	in *= 10;
	o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_z_rps = in;
	return 0;
}

int print_can_0x102_RAW_Accel_Gyro(const can_obj_sbm_network_definition_h_t *o, FILE *output) {
	assert(o);
	assert(output);
	int r = 0;
	r = print_helper(r, fprintf(output, "Raw_Accel_x_mps2 = (wire: %.0f)\n", (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_x_mps2)));
	r = print_helper(r, fprintf(output, "Raw_Accel_y_mps2 = (wire: %.0f)\n", (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_y_mps2)));
	r = print_helper(r, fprintf(output, "Raw_Accel_z_mps2 = (wire: %.0f)\n", (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Accel_z_mps2)));
	r = print_helper(r, fprintf(output, "Raw_Gyro_x_rps = (wire: %.0f)\n", (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_x_rps)));
	r = print_helper(r, fprintf(output, "Raw_Gyro_y_rps = (wire: %.0f)\n", (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_y_rps)));
	r = print_helper(r, fprintf(output, "Raw_Gyro_z_rps = (wire: %.0f)\n", (double)(o->can_0x102_RAW_Accel_Gyro.Raw_Gyro_z_rps)));
	return r;
}

static int pack_can_0x103_RAW_Mag(can_obj_sbm_network_definition_h_t *o, uint64_t *data) {
	assert(o);
	assert(data);
	register uint64_t x;
	register uint64_t i = 0;
	/* Raw_Mag_x_uT: start-bit 0, length 8, endianess intel, scaling 1, offset 0 */
	x = ((uint8_t)(o->can_0x103_RAW_Mag.Raw_Mag_x_uT)) & 0xff;
	i |= x;
	/* Raw_Mag_y_uT: start-bit 8, length 8, endianess intel, scaling 1, offset 0 */
	x = ((uint8_t)(o->can_0x103_RAW_Mag.Raw_Mag_y_uT)) & 0xff;
	x <<= 8; 
	i |= x;
	/* Raw_Mag_z_uT: start-bit 16, length 8, endianess intel, scaling 1, offset 0 */
	x = ((uint8_t)(o->can_0x103_RAW_Mag.Raw_Mag_z_uT)) & 0xff;
	x <<= 16; 
	i |= x;
	*data = (i);
	o->can_0x103_RAW_Mag_tx = 1;
	return 0;
}

static int unpack_can_0x103_RAW_Mag(can_obj_sbm_network_definition_h_t *o, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp) {
	assert(o);
	assert(dlc <= 8);
	register uint64_t x;
	register uint64_t i = (data);
	if (dlc < 8)
		return -1;
	/* Raw_Mag_x_uT: start-bit 0, length 8, endianess intel, scaling 1, offset 0 */
	x = i & 0xff;
	o->can_0x103_RAW_Mag.Raw_Mag_x_uT = x;
	/* Raw_Mag_y_uT: start-bit 8, length 8, endianess intel, scaling 1, offset 0 */
	x = (i >> 8) & 0xff;
	o->can_0x103_RAW_Mag.Raw_Mag_y_uT = x;
	/* Raw_Mag_z_uT: start-bit 16, length 8, endianess intel, scaling 1, offset 0 */
	x = (i >> 16) & 0xff;
	o->can_0x103_RAW_Mag.Raw_Mag_z_uT = x;
	o->can_0x103_RAW_Mag_rx = 1;
	o->can_0x103_RAW_Mag_time_stamp_rx = time_stamp;
	return 0;
}

int decode_can_0x103_Raw_Mag_x_uT(const can_obj_sbm_network_definition_h_t *o, int8_t *out) {
	assert(o);
	assert(out);
	int8_t rval = (int8_t)(o->can_0x103_RAW_Mag.Raw_Mag_x_uT);
	*out = rval;
	return 0;
}

int encode_can_0x103_Raw_Mag_x_uT(can_obj_sbm_network_definition_h_t *o, int8_t in) {
	assert(o);
	o->can_0x103_RAW_Mag.Raw_Mag_x_uT = in;
	return 0;
}

int decode_can_0x103_Raw_Mag_y_uT(const can_obj_sbm_network_definition_h_t *o, int8_t *out) {
	assert(o);
	assert(out);
	int8_t rval = (int8_t)(o->can_0x103_RAW_Mag.Raw_Mag_y_uT);
	*out = rval;
	return 0;
}

int encode_can_0x103_Raw_Mag_y_uT(can_obj_sbm_network_definition_h_t *o, int8_t in) {
	assert(o);
	o->can_0x103_RAW_Mag.Raw_Mag_y_uT = in;
	return 0;
}

int decode_can_0x103_Raw_Mag_z_uT(const can_obj_sbm_network_definition_h_t *o, int8_t *out) {
	assert(o);
	assert(out);
	int8_t rval = (int8_t)(o->can_0x103_RAW_Mag.Raw_Mag_z_uT);
	*out = rval;
	return 0;
}

int encode_can_0x103_Raw_Mag_z_uT(can_obj_sbm_network_definition_h_t *o, int8_t in) {
	assert(o);
	o->can_0x103_RAW_Mag.Raw_Mag_z_uT = in;
	return 0;
}

int print_can_0x103_RAW_Mag(const can_obj_sbm_network_definition_h_t *o, FILE *output) {
	assert(o);
	assert(output);
	int r = 0;
	r = print_helper(r, fprintf(output, "Raw_Mag_x_uT = (wire: %.0f)\n", (double)(o->can_0x103_RAW_Mag.Raw_Mag_x_uT)));
	r = print_helper(r, fprintf(output, "Raw_Mag_y_uT = (wire: %.0f)\n", (double)(o->can_0x103_RAW_Mag.Raw_Mag_y_uT)));
	r = print_helper(r, fprintf(output, "Raw_Mag_z_uT = (wire: %.0f)\n", (double)(o->can_0x103_RAW_Mag.Raw_Mag_z_uT)));
	return r;
}

static int pack_can_0x104_CAN_Logger_End(can_obj_sbm_network_definition_h_t *o, uint64_t *data) {
	assert(o);
	assert(data);
	register uint64_t x;
	register uint64_t i = 0;
	/* CAN_Logger: start-bit 0, length 1, endianess intel, scaling 1, offset 0 */
	x = ((uint8_t)(o->can_0x104_CAN_Logger_End.CAN_Logger)) & 0x1;
	i |= x;
	*data = (i);
	o->can_0x104_CAN_Logger_End_tx = 1;
	return 0;
}

static int unpack_can_0x104_CAN_Logger_End(can_obj_sbm_network_definition_h_t *o, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp) {
	assert(o);
	assert(dlc <= 8);
	register uint64_t x;
	register uint64_t i = (data);
	if (dlc < 8)
		return -1;
	/* CAN_Logger: start-bit 0, length 1, endianess intel, scaling 1, offset 0 */
	x = i & 0x1;
	o->can_0x104_CAN_Logger_End.CAN_Logger = x;
	o->can_0x104_CAN_Logger_End_rx = 1;
	o->can_0x104_CAN_Logger_End_time_stamp_rx = time_stamp;
	return 0;
}

int decode_can_0x104_CAN_Logger(const can_obj_sbm_network_definition_h_t *o, uint8_t *out) {
	assert(o);
	assert(out);
	uint8_t rval = (uint8_t)(o->can_0x104_CAN_Logger_End.CAN_Logger);
	*out = rval;
	return 0;
}

int encode_can_0x104_CAN_Logger(can_obj_sbm_network_definition_h_t *o, uint8_t in) {
	assert(o);
	o->can_0x104_CAN_Logger_End.CAN_Logger = in;
	return 0;
}

int print_can_0x104_CAN_Logger_End(const can_obj_sbm_network_definition_h_t *o, FILE *output) {
	assert(o);
	assert(output);
	int r = 0;
	r = print_helper(r, fprintf(output, "CAN_Logger = (wire: %.0f)\n", (double)(o->can_0x104_CAN_Logger_End.CAN_Logger)));
	return r;
}

static int pack_can_0x105_CAN_Logger_Begin(can_obj_sbm_network_definition_h_t *o, uint64_t *data) {
	assert(o);
	assert(data);
	register uint64_t x;
	register uint64_t i = 0;
	/* CAN_Logger: start-bit 0, length 1, endianess intel, scaling 1, offset 0 */
	x = ((uint8_t)(o->can_0x105_CAN_Logger_Begin.CAN_Logger)) & 0x1;
	i |= x;
	*data = (i);
	o->can_0x105_CAN_Logger_Begin_tx = 1;
	return 0;
}

static int unpack_can_0x105_CAN_Logger_Begin(can_obj_sbm_network_definition_h_t *o, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp) {
	assert(o);
	assert(dlc <= 8);
	register uint64_t x;
	register uint64_t i = (data);
	if (dlc < 8)
		return -1;
	/* CAN_Logger: start-bit 0, length 1, endianess intel, scaling 1, offset 0 */
	x = i & 0x1;
	o->can_0x105_CAN_Logger_Begin.CAN_Logger = x;
	o->can_0x105_CAN_Logger_Begin_rx = 1;
	o->can_0x105_CAN_Logger_Begin_time_stamp_rx = time_stamp;
	return 0;
}

int decode_can_0x105_CAN_Logger(const can_obj_sbm_network_definition_h_t *o, uint8_t *out) {
	assert(o);
	assert(out);
	uint8_t rval = (uint8_t)(o->can_0x105_CAN_Logger_Begin.CAN_Logger);
	*out = rval;
	return 0;
}

int encode_can_0x105_CAN_Logger(can_obj_sbm_network_definition_h_t *o, uint8_t in) {
	assert(o);
	o->can_0x105_CAN_Logger_Begin.CAN_Logger = in;
	return 0;
}

int print_can_0x105_CAN_Logger_Begin(const can_obj_sbm_network_definition_h_t *o, FILE *output) {
	assert(o);
	assert(output);
	int r = 0;
	r = print_helper(r, fprintf(output, "CAN_Logger = (wire: %.0f)\n", (double)(o->can_0x105_CAN_Logger_Begin.CAN_Logger)));
	return r;
}

int unpack_message(can_obj_sbm_network_definition_h_t *o, const unsigned long id, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp) {
	assert(o);
	assert(id < (1ul << 29)); /* 29-bit CAN ID is largest possible */
	assert(dlc <= 8);         /* Maximum of 8 bytes in a CAN packet */
	switch (id) {
	case 0x100: return unpack_can_0x100_IMU(o, data, dlc, time_stamp);
	case 0x101: return unpack_can_0x101_GPS(o, data, dlc, time_stamp);
	case 0x102: return unpack_can_0x102_RAW_Accel_Gyro(o, data, dlc, time_stamp);
	case 0x103: return unpack_can_0x103_RAW_Mag(o, data, dlc, time_stamp);
	case 0x104: return unpack_can_0x104_CAN_Logger_End(o, data, dlc, time_stamp);
	case 0x105: return unpack_can_0x105_CAN_Logger_Begin(o, data, dlc, time_stamp);
	default: break; 
	}
	return -1; 
}

int pack_message(can_obj_sbm_network_definition_h_t *o, const unsigned long id, uint64_t *data) {
	assert(o);
	assert(id < (1ul << 29)); /* 29-bit CAN ID is largest possible */
	switch (id) {
	case 0x100: return pack_can_0x100_IMU(o, data);
	case 0x101: return pack_can_0x101_GPS(o, data);
	case 0x102: return pack_can_0x102_RAW_Accel_Gyro(o, data);
	case 0x103: return pack_can_0x103_RAW_Mag(o, data);
	case 0x104: return pack_can_0x104_CAN_Logger_End(o, data);
	case 0x105: return pack_can_0x105_CAN_Logger_Begin(o, data);
	default: break; 
	}
	return -1; 
}

int print_message(const can_obj_sbm_network_definition_h_t *o, const unsigned long id, FILE *output) {
	assert(o);
	assert(id < (1ul << 29)); /* 29-bit CAN ID is largest possible */
	assert(output);
	switch (id) {
	case 0x100: return print_can_0x100_IMU(o, output);
	case 0x101: return print_can_0x101_GPS(o, output);
	case 0x102: return print_can_0x102_RAW_Accel_Gyro(o, output);
	case 0x103: return print_can_0x103_RAW_Mag(o, output);
	case 0x104: return print_can_0x104_CAN_Logger_End(o, output);
	case 0x105: return print_can_0x105_CAN_Logger_Begin(o, output);
	default: break; 
	}
	return -1; 
}

