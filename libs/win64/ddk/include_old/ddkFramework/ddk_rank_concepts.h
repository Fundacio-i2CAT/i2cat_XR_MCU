#pragma once

#define IS_SAME_RANK(_RANK1,_RANK2) \
	typename std::enable_if<(_RANK1==_RANK2)>::type