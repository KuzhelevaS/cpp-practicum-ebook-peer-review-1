#include <iostream>
#include <vector>
#include <iomanip>
#include <optional>

class Ebook {
public:
	// Прибавляем 1 к макс. значению, чтобы при обращении по индексу не вычитать ее
	Ebook(int max_users, int max_pages) 
		: user_progress_(max_users + 1, std::nullopt), page_statistic_(max_pages + 1) {}
	
	void AddStatistic(int user, int page) {
		if (user_progress_.at(user).has_value()) {
			--page_statistic_.at(user_progress_.at(user).value());
		} else {
			++user_count;
		}
	   
		user_progress_.at(user) = page;
		++page_statistic_.at(page);
	}
	
	double ComputeProgress(int user) const {
		if (user >= user_progress_.size() || !user_progress_.at(user).has_value()) {
			return 0;
		}
		if (user_count == 1) {
			return 1;
		}
		int prev_users = 0;
		for (int i = 1; i < user_progress_.at(user).value(); ++i) {
			prev_users += page_statistic_.at(i);
		}
		return prev_users * 1.0 / (user_count - 1);
	}
private:
	std::vector<std::optional<int>> user_progress_;
	std::vector<int> page_statistic_;
	int user_count = 0;
};

namespace requests {
	enum class RequestType {
		READ,
		CHEER,
		UNKNOWN
	};
	struct Request {
		RequestType type;
		int user;
		int page;
	};
	
	namespace detail {
		RequestType StringToType(std::string_view str) {
			if (str == "READ") {
				return RequestType::READ;
			} else if (str == "CHEER") {
				return RequestType::CHEER;
			} else {
				return RequestType::UNKNOWN;
			}
		}
		
		Request ReadItem(std::istream & in) {
			std::string type_str;
			in >> type_str;
			
			Request result;
			result.type = StringToType(type_str);
			
			switch(result.type) {
				case RequestType::READ :
					in >> result.user >> result.page;
					break;
				case RequestType::CHEER :
					in >> result.user;
					break;
				default:
					break;
			}
			
			return result;
		}
		void PrintItem(std::ostream & out, double data) {
			out << std::setprecision(6) << data << std::endl;
		}
	}
	
	std::vector<Request> Read() {
		int q;
		std::cin >> q;
		
		std::vector<Request> requests;
		requests.reserve(q);
		
		for (int i = 0; i < q; ++i) {
			requests.push_back(std::move(detail::ReadItem(std::cin)));
		}
		return requests;
	}
	
	std::vector<double> Handle(Ebook & ebook, const std::vector<Request> & requests) {
		std::vector<double> result;
		for(auto request : requests) {
			switch(request.type) {
				case RequestType::READ :
					ebook.AddStatistic(request.user, request.page);
					break;
				case RequestType::CHEER :
					result.push_back(ebook.ComputeProgress(request.user));
					break;
				default:
					break;
			}
		}
		return result;
	}
	void Print(const std::vector<double> & results) {
		for(auto result : results) {
			detail::PrintItem(std::cout, result);
		}
	}
}

int main() {
	using namespace requests;
	Ebook ebook(100'000, 1'000);
	Print(Handle(ebook, Read()));
	
	return 0;
}
