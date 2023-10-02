// DataRefManager.cpp

#include "DataRefManager.h"
#include <cstring>
#include <string>
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"



std::vector<DataRefItem> DataRefManager::dataRefs = {
	// Position Information
	{"Position", "Latitude", "sim/flightmodel/position/latitude", "deg", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Position", "Longitude", "sim/flightmodel/position/longitude", "deg", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Position", "Longitude", "sim/flightmodel/position/longitude", "deg", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Position", "Altitude", "sim/flightmodel/position/elevation", "m", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Position", "Course (Track)", "sim/cockpit2/gauges/indicators/ground_track_mag_copilot", "deg", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},

	// Attitude Information
	{"Attitude", "Pitch", "sim/flightmodel/position/theta", "deg", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Attitude", "Roll", "sim/flightmodel/position/phi", "deg", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Attitude", "Heading", "sim/flightmodel/position/psi", "deg", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Attitude", "Quaternion", "sim/flightmodel/position/q", "", 1, DRT_FLOAT_ARRAY, std::function<std::vector<float>(const char*)>(getFloatArray)},

	// Acceleration Information
	{"Acceleration (NED)", "X-Acc", "sim/flightmodel/position/local_ax", "m/s^2", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Acceleration (NED)", "Y-Acc", "sim/flightmodel/position/local_ay", "m/s^2", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Acceleration (NED)", "Z-Acc", "sim/flightmodel/position/local_az", "m/s^2", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},

	// Acceleration Information
	{"Acceleration (BODY)", "X-Acc", "sim/flightmodel/forces/g_axil", "m/s^2", g_earth, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Acceleration (BODY)", "Y-Acc", "sim/flightmodel/forces/g_side", "m/s^2", g_earth, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Acceleration (BODY)", "Z-Acc", "sim/flightmodel/forces/g_nrml", "m/s^2", g_earth, DRT_FLOAT, std::function<float(const char*)>(getFloat)},

	// Ground Speed Information
	{"Ground Speed", "Vx", "sim/flightmodel/position/local_vx", "m/s", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Ground Speed", "Vy", "sim/flightmodel/position/local_vy", "m/s", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Ground Speed", "Vz", "sim/flightmodel/position/local_vz", "m/s", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},

	// Airspeed Information
	{"Airspeed", "IAS", "sim/flightmodel/position/indicated_airspeed", "m/s", 0.514444, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Airspeed", "TAS", "sim/flightmodel/position/true_airspeed", "m/s", 0.514444, DRT_FLOAT, std::function<float(const char*)>(getFloat)},

	// Pressure Information
	{"Pressure", "Pressure Altitude", "sim/flightmodel2/position/pressure_altitude", "m", 0.3048, DRT_DOUBLE, std::function<double(const char*)>(getDouble)},
	{"Pressure", "Absolute Pressure", "sim/weather/barometer_current_inhg", "hPa", 33.8639, DRT_FLOAT, std::function<float(const char*)>(getFloat)},

	// Angular Velocities
	{"Angular Velocity", "P", "sim/flightmodel/position/Prad", "rad/s", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Angular Velocity", "Q", "sim/flightmodel/position/Qrad", "rad/s", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Angular Velocity", "R", "sim/flightmodel/position/Rrad", "rad/s", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},

	// Temperature
	{"Temperature", "Outside Air Temp", "sim/cockpit2/temperature/outside_air_temp_degc", "degC", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},

	// Time Information
	{"Time", "Total Flight Time", "sim/time/total_flight_time_sec", "s", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
	{"Time", "Zulu Time", "sim/time/zulu_time_sec", "s", 1, DRT_FLOAT, std::function<float(const char*)>(getFloat)},
};


void DataRefManager::drawDataRefs(XPLMWindowID in_window_id, int l, int t, float col_white[],int lineOffset) {
	

	const char* currentCategory = nullptr;
	for (const auto& item : dataRefs) {
		if (currentCategory == nullptr || strcmp(currentCategory, item.category) != 0) {
			currentCategory = item.category;
			XPLMDrawString(col_white, l + 10, t - lineOffset, (char*)currentCategory, NULL, xplmFont_Proportional);
			lineOffset += 20; // Increment line offset for next items
		}

		std::string valueStr; // string to store the formatted value
		if (item.type == DRT_FLOAT) {
			float value = getFloat(item.dataRef) * item.multiplier;
			valueStr = std::to_string(value);
		}
		else if (item.type == DRT_DOUBLE) {
			double value = getDouble(item.dataRef) * item.multiplier;
			valueStr = std::to_string(value);
		}
		else if (item.type == DRT_INT) {
			int value = getInt(item.dataRef) * static_cast<int>(item.multiplier);
			valueStr = std::to_string(value);
		}
		else if (item.type == DRT_FLOAT_ARRAY) {
			std::vector<float> arrayValues = getFloatArray(item.dataRef);
			valueStr = arrayToString(arrayValues);
		}

		char buf[512];  // Ensure this buffer is large enough to hold the entire string.
		snprintf(buf, sizeof(buf), "  %s: %s %s", item.title, valueStr.c_str(), item.unit);
		XPLMDrawString(col_white, l + 10, t - lineOffset, buf, NULL, xplmFont_Proportional);
		lineOffset += 20; // Increment line offset for next items
	}
}



float DataRefManager::getFloat(const char* dataRef) {
	XPLMDataRef ref = XPLMFindDataRef(dataRef);
	if (ref) return XPLMGetDataf(ref);
	return 0; // or some other suitable default or error value
}

double DataRefManager::getDouble(const char* dataRef) {
	XPLMDataRef ref = XPLMFindDataRef(dataRef);
	if (ref) return XPLMGetDatad(ref);
	return 0; // or some other suitable default or error value
}

int DataRefManager::getInt(const char* dataRef) {
	XPLMDataRef ref = XPLMFindDataRef(dataRef);
	if (ref) return XPLMGetDatai(ref);
	return 0; // or some other suitable default or error value
}

std::vector<float> DataRefManager::getFloatArray(const char* dataRefName) {
	// This is just a placeholder; replace with the actual SDK call to get float array dataRef.
	XPLMDataRef dataRef = XPLMFindDataRef(dataRefName);
	int arraySize = XPLMGetDatavf(dataRef, NULL, 0, 0); // To get the array size
	std::vector<float> values(arraySize);
	XPLMGetDatavf(dataRef, values.data(), 0, arraySize); // To get the array data
	return values;
}

std::string DataRefManager::arrayToString(const std::vector<float>& array) {
	std::string result = "[";
	for (size_t i = 0; i < array.size(); ++i) {
		result += std::to_string(array[i]);
		if (i < array.size() - 1)
			result += ",";
	}
	result += "]";
	return result;
}
