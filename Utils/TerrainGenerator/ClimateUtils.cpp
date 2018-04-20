#include "ClimateUtils.h"

// in kg/m^3 from -50 degree Celsius to 150 degree Celsius
const float AirHumidityTable[] = {
	0.00006	,// -50
	0.00007	,// -49
	0.00008	,// -48
	0.00009	,// -47
	0.00010	,// -46
	0.00011	,// -45
	0.00012	,// -44
	0.00013	,// -43
	0.00014	,// -42
	0.00016	,// -41
	0.00018	,// -40
	0.00020	,// -39
	0.00022	,// -38
	0.00024	,// -37
	0.00026	,// -36
	0.00029	,// -35
	0.00032	,// -34
	0.00035	,// -33
	0.00038	,// -32
	0.00042	,// -31
	0.00046	,// -30
	0.00050	,// -29
	0.00055	,// -28
	0.00060	,// -27
	0.00065	,// -26
	0.00071	,// -25
	0.00077	,// -24
	0.00084	,// -23
	0.00091	,// -22
	0.00099	,// -21
	0.00108	,// -20
	0.00117	,// -19
	0.00127	,// -18
	0.00137	,// -17
	0.00149	,// -16
	0.00161	,// -15
	0.00174	,// -14
	0.00188	,// -13
	0.00203	,// -12
	0.00219	,// -11
	0.00236	,// -10
	0.00255	,// -9
	0.00274	,// -8
	0.00295	,// -7
	0.00317	,// -6
	0.00341	,// -5
	0.00366	,// -4
	0.00393	,// -3
	0.00422	,// -2
	0.00453	,// -1
	0.00485	,// 0
	0.00519	,// 1
	0.00556	,// 2
	0.00595	,// 3
	0.00636	,// 4
	0.00679	,// 5
	0.00725	,// 6
	0.00774	,// 7
	0.00826	,// 8
	0.00880	,// 9
	0.00938	,// 10
	0.00999	,// 11
	0.01064	,// 12
	0.01132	,// 13
	0.01204	,// 14
	0.01280	,// 15
	0.01360	,// 16
	0.01444	,// 17
	0.01533	,// 18
	0.01626	,// 19
	0.01724	,// 20
	0.01828	,// 21
	0.01936	,// 22
	0.02051	,// 23
	0.02171	,// 24
	0.02297	,// 25
	0.02429	,// 26
	0.02568	,// 27
	0.02714	,// 28
	0.02866	,// 29
	0.03026	,// 30
	0.03194	,// 31
	0.03370	,// 32
	0.03553	,// 33
	0.03746	,// 34
	0.03947	,// 35
	0.04158	,// 36
	0.04378	,// 37
	0.04607	,// 38
	0.04848	,// 39
	0.05098	,// 40
	0.05360	,// 41
	0.05633	,// 42
	0.05918	,// 43
	0.06215	,// 44
	0.06525	,// 45
	0.06848	,// 46
	0.07184	,// 47
	0.07534	,// 48
	0.07899	,// 49
	0.08278	,// 50
	0.08673	,// 51
	0.09084	,// 52
	0.09511	,// 53
	0.09955	,// 54
	0.10417	,// 55
	0.10896	,// 56
	0.11394	,// 57
	0.11911	,// 58
	0.12448	,// 59
	0.13005	,// 60
	0.13583	,// 61
	0.14182	,// 62
	0.14803	,// 63
	0.15447	,// 64
	0.16115	,// 65
	0.16807	,// 66
	0.17523	,// 67
	0.18265	,// 68
	0.19033	,// 69
	0.19828	,// 70
	0.20650	,// 71
	0.21501	,// 72
	0.22381	,// 73
	0.23291	,// 74
	0.24231	,// 75
	0.25203	,// 76
	0.26207	,// 77
	0.27245	,// 78
	0.28316	,// 79
	0.29422	,// 80
	0.30564	,// 81
	0.31743	,// 82
	0.32959	,// 83
	0.34213	,// 84
	0.35507	,// 85
	0.36841	,// 86
	0.38216	,// 87
	0.39633	,// 88
	0.41094	,// 89
	0.42598	,// 90
	0.44148	,// 91
	0.45744	,// 92
	0.47388	,// 93
	0.49079	,// 94
	0.50821	,// 95
	0.52612	,// 96
	0.54455	,// 97
	0.56351	,// 98
	0.58301	,// 99
	0.60306	,// 100
	0.62367	,// 101
	0.64485	,// 102
	0.66662	,// 103
	0.68899	,// 104
	0.71196	,// 105
	0.73556	,// 106
	0.75979	,// 107
	0.78467	,// 108
	0.81021	,// 109
	0.83642	,// 110
	0.86331	,// 111
	0.89091	,// 112
	0.91922	,// 113
	0.94825	,// 114
	0.97803	,// 115
	1.00855	,// 116
	1.03985	,// 117
	1.07192	,// 118
	1.10479	,// 119
	1.13848	,// 120
	1.17298	,// 121
	1.20833	,// 122
	1.24453	,// 123
	1.28159	,// 124
	1.31955	,// 125
	1.35840	,// 126
	1.39816	,// 127
	1.43886	,// 128
	1.48050	,// 129
	1.52310	,// 130
	1.56668	,// 131
	1.61125	,// 132
	1.65683	,// 133
	1.70344	,// 134
	1.75109	,// 135
	1.79980	,// 136
	1.84958	,// 137
	1.90045	,// 138
	1.95244	,// 139
	2.00554	,// 140
	2.05980	,// 141
	2.11521	,// 142
	2.17180	,// 143
	2.22958	,// 144
	2.28858	,// 145
	2.34880	,// 146
	2.41028	,// 147
	2.47302	,// 148
	2.53705	,// 149
	2.60237	// 150
};

float ClimateUtils::GetMaxAirHumidity(int32_t temperature)
{
	if(temperature < CLIMATE_MIN_TEMPERATURE_KELVIN) temperature = CLIMATE_MIN_TEMPERATURE_KELVIN;
	if(temperature > CLIMATE_MAX_TEMPERATURE_KELVIN) temperature = CLIMATE_MAX_TEMPERATURE_KELVIN;
	
	// array correction
	temperature = temperature - CLIMATE_MIN_TEMPERATURE_KELVIN;
	
	return AirHumidityTable[temperature];
}

float ClimateUtils::GetDewPointTemperature(float relHum)
{
	int32_t chunkSize = 100;
	int32_t temp = 100;
	while(chunkSize > 1)
	{
		if(relHum > AirHumidityTable[temp])
		{
			temp += temp/2;
		} else {
			temp -= temp/2;
		}
		chunkSize = chunkSize/2;
	}
	
	return (float)temp;
}

float ClimateUtils::GetWaterDensity(int32_t temperature, float salinity)
{
	if(temperature < 273) temperature = 273;
	if(temperature > 373) temperature = 373;
	
	temperature = temperature - 273;
	
	float koef = 0.8*salinity + 998.52;
	return (-0.3663*(float)(temperature*temperature) + 1.6315*(float)(temperature) + koef);
}

float ClimateUtils::GetAirDensity(int32_t temperature)
{
	if(temperature < CLIMATE_MIN_TEMPERATURE_KELVIN) temperature = CLIMATE_MIN_TEMPERATURE_KELVIN;
	if(temperature > CLIMATE_MAX_TEMPERATURE_KELVIN) temperature = CLIMATE_MAX_TEMPERATURE_KELVIN;
	
	temperature = temperature - 273;
	
	return (-0.0047 * (float)(temperature) + 1.32);
}

float ClimateUtils::GetSpecificHeat(CellContent::ContentType contType)
{
	switch(contType)
	{
		case CellContent::AIR:
			return 1020.0;
		case CellContent::WATER:
			return 4020.0;
		case CellContent::ICE:
			return 2093.4;
		case CellContent::GROUND:
			return 940.0;
		default:
			return 0.0;
	}
}

float ClimateUtils::GetThermalConductivity(CellContent::ContentType contType)
{
	switch(contType)
	{
		case CellContent::AIR:
			return 0.026;
		case CellContent::WATER:
			return 0.6;
		case CellContent::ICE:
			return 2.3;
		case CellContent::GROUND:
			return 0.950;
		default:
			return 0.0;
	}
}





