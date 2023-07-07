src := $(wildcard *.cpp)
exclude_files := testLogFile.cpp testBase.cpp  # 要排除的文件名，使用空格分隔
filtered_src := $(filter-out $(exclude_files), $(src))

obj := $(patsubst %.cpp, %.o, $(filtered_src))
Args := -Wall -std=c++11 -pthread -g

all:out

out:$(obj)
	g++ $^ -o $@ $(Args)

%.o:%.cpp
	g++ -c $< -o $@ $(Args)

clean:
	-rm -rf $(obj) out