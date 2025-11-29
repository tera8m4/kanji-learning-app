#include "controller.h"
#include "kanji.h"

namespace kanji
{
	std::vector<KanjiData> Controller::GetReviewKanjis()
	{
		KanjiData data = {
		    .id = 1,
		    .kanji = "日",
		    .meaning = "sun, day",
		    .examples = {{.word = "毎日", .reading = "まいにち"}}
		};

		return {data};
	}

	void Controller::SetAnswers(const std::vector<std::pair<int, bool>>& in_answers)
	{
	}
} // namespace kanji
