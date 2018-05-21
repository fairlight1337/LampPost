#ifndef LAMPPOST_IDENTIFIABLE_H
#define LAMPPOST_IDENTIFIABLE_H


#include <string>


namespace lp {
	class Identifiable {
	private:
		std::string mIdentifier;

	public:
		Identifiable(std::string identifier);
		virtual ~Identifiable();

		std::string GetIdentifier();
	};
}

#endif //LAMPPOST_IDENTIFIABLE_H
