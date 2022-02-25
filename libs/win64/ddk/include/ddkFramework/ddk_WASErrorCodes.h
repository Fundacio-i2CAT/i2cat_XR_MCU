#ifndef _WAS_ERROR_CODES_H_
#define _WAS_ERROR_CODES_H_

#define LATITUDE_OUT_OF_RANGE_ERROR			-3010
#define LATITUDE_OUT_OF_RANGE_DESCR			"Latitude out of range"

#define LONGITUDE_OUT_OF_RANGE_ERROR		-3011
#define LONGITUDE_OUT_OF_RANGE_DESCR		"Longitude out of range"

#define CD_INVALID_PTP_POINTER_ERROR				-3013
#define CD_INVALID_PTP_POINTER_ERROR_DESCR			"Invalid PTP pointer"

#define CD_INVALID_SIMINFO_POINTER_ERROR			-3014
#define CD_INVALID_SIMINFO_POINTER_ERROR_DESCR	"Invalid SimulationInfo pointer"

#define CD_INVALID_NAVINFO_POINTER_ERROR			-3015
#define CD_INVALID_NAVINFO_POINTER_ERROR_DESCR		"Invalid NavInfo pointer"

#define CD_INVALID_CONFLICLIST_POINTER_ERROR		-3016
#define CD_INVALID_CONFLICLIST_POINTER_ERROR_DESCR	"Invalid conflict list pointer"

#define CD_INVALID_POLYGON_POINTER_ERROR			-3017
#define CD_INVALID_POLYGON_POINTER_ERROR_DESCR		"Invalid polygon pointer"

#define STRING_TO_POLYGON_INCORRECT_POLY_PARAM_ERROR	-3020
#define STRING_TO_POLYGON_INCORRECT_POLY_PARAM_DESCR	"Wrong parameter (pointer to polygon) in stringToPolygon"

#define STRING_TO_POLYGON_INCORRECT_STRING_PARAM_ERROR	-3021
#define STRING_TO_POLYGON_INCORRECT_STRING_PARAM_DESCR	"Wrong parameter (pointer to string) in stringToPolygon"

#define STRING_TO_POLYGON_INCORRECT_NUMBER_OF_POINTS_PARAM_ERROR -3022
#define STRING_TO_POLYGON_INCORRECT_NUMBER_OF_POINTS_PARAM_DESCR "Wrong parameter (number of polygon points) in stringToPolygon"

#define STRING_TO_POLYGON_POLYGON_POINTS_ERROR	-3023
#define STRING_TO_POLYGON_POLYGON_POINTS_DESCR	"Unable to add a new point to the polygon in stringToPolygon"

#define STRING_TO_TIMESTAMP_INCORRECT_CTIME_PARAM_ERROR	 -3024
#define STRING_TO_TIMESTAMP_INCORRECT_CTIME_PARAM_DESCR	 "Wrong parameter (pointer to CTime) in stringToTimestamp"

#define STRING_TO_TIMESTAMP_INCORRECT_STRING_PARAM_ERROR  -3025
#define STRING_TO_TIMESTAMP_INCORRECT_STRING_PARAM_DESCR "Wrong parameter (pointer to string) in stringToTimestamp"

#define CALCULATEPOLYEXTREMES_INCORRECT_POLY_PARAM_ERROR	-3028
#define CALCULATEPOLYEXTREMES_INCORRECT_POLY_PARAM_DESCR	"Wrong parameter (pointer to polygon) in calculatePolyExtremes"

#define CALCULATEPOLYEXTREMES_MATH_ERROR			-3029
#define CALCULATEPOLYEXTREMES_MATH_DESCR			"Warning in calculatePolyExtremes computations"

#define LATLON2XYMERCATOR_MATH_ERROR				-3030
#define LATLON2XYMERCATOR_MATH_DESCR				"Error in LatLon2xyMercator computations"

#define POLYGON_POINTER_NOT_INITIZALIZED_ERROR		-3031
#define POLYGON_POINTER_NOT_INITIZALIZED_ERROR_DESCR	"List of points in Polygon not initialized in init"

#define POLYGON_POINTER_NOT_INITIZALIZED_ERROR		-3031
#define POLYGON_POINTER_NOT_INITIZALIZED_DESCR_ADD	"List of points in Polygon not initialized in add"

#define STRING_TO_TIMESTAMP_DATE_FORMAT_ERROR  -3033
#define STRING_TO_TIMESTAMP_DATE_FORMAT_DESCR "Wrong date format in stringToTimestamp"

#define TIMEPARAM_PTPTIMESTAMPWITHDATE_ERROR  -3037
#define TIMEPARAM_PTPTIMESTAMPWITHDATE_DESCR "Time parameter out of range in ptpTimeStampWithDate"

#define HORIZONTAL_NGA_WRONG_POINTER_ERROR  -3038
#define HORIZONTAL_NGA_WRONG_POINTER_DESCR "Wrong pointer to left or right arc in ComputeHorizontalNoGoArc"

#define DISTANCE_WRONG_POINTER_ERROR  -3039
#define DISTANCE_WRONG_POINTER_DESCR "Wrong pointer to distance in ComputeHorizontalNoGoArc"

#define SDCPOLYGON_NUMBER_OF_POINTS_EXCEED_MAX_RANGE_ERROR  -3040
#define SDCPOLYGON_NUMBER_OF_POINTS_EXCEED_MAX_RANGE_DESCR  "Number of points in the polygon exceed the maximum allowed"

#define POLYGON_POINTER_NOT_INITIZALIZED_ERROR						-3031
#define POLYGON_POINTER_NOT_INITIZALIZED_ERROR_DESCR				"List of points in Polygon not initialized in init"

#define STRING_TO_LINEFRONT_INCORRECT_LINE_PARAM_ERROR	-3032
#define STRING_TO_LINEFRONT_INCORRECT_LINE_PARAM_DESCR	"Wrong parameter (pointer to LineFront) in stringToLineFront"

#define STRING_TO_LINEFRONT_INCORRECT_STRING_PARAM_ERROR	-3033
#define STRING_TO_LINEFRONT_INCORRECT_STRING_PARAM_DESCR	"Wrong parameter (pointer to string) in stringToLineFront"

#define STRING_TO_LINEFRONT_INCORRECT_NUMBER_OF_POINTS_PARAM_ERROR -3034
#define STRING_TO_LINEFRONT_INCORRECT_NUMBER_OF_POINTS_PARAM_DESCR "Wrong parameter (number of LineFront points) in stringToLineFront"

#define STRING_TO_LINEFRONT_LINEFRONT_POINTS_ERROR	-3035
#define STRING_TO_LINEFRONT_LINEFRONT_POINTS_DESCR	"Unable to add a new point to the LineFront in stringToLineFront"

#define STRING_TO_COORDINATES_INCORRECT_COORD_PARAM_ERROR	-3036
#define STRING_TO_COORDINATES_INCORRECT_COORD_PARAM_DESCR	"Wrong parameter (pointer to cooridnates) in stringToCoordinates"

#define STRING_TO_COORDINATES_INCORRECT_STRING_PARAM_ERROR	-3037
#define STRING_TO_COORDINATES_INCORRECT_STRING_PARAM_DESCR	"Wrong parameter (pointer to string) in stringToCoordinates"

#endif //_WAS_ERROR_CODES_H_
