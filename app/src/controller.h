#pragma once

#include "kanji.h"
#include <vector>

namespace kanji
{
	class Controller
	{
	public:
		std::vector<KanjiData> GetReviewKanjis();
		void SetAnswers(const std::vector<std::pair<int, bool>>& in_answers);
	};
} // namespace kanji
