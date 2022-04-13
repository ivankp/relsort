NAME = relsort

all: $(NAME)

%: %.cc
	$(CXX) -Wall -O3 -flto -o $@ $^
