SOURCE_DIR:=${shell dirname ${realpath ${lastword ${MAKEFILE_LIST}}}}
BUILD_DIR := ${SOURCE_DIR}/build

GIT := git
CMAKE := cmake
CLANG_FORMAT := clang-format

all: playground

dev: format playground

clean:
	rm -rf ${BUILD_DIR}

playground: configure-playground build-playground

configure-playground:
	mkdir -p ${BUILD_DIR}
	${CMAKE} \
		-H${SOURCE_DIR} \
		-B${BUILD_DIR} \
		-G Ninja \
		-DCMAKE_BUILD_TYPE=Debug

build-playground:
	${CMAKE} --build ${BUILD_DIR}

format:
	${CLANG_FORMAT} -i \
		$(shell \
			find ${SOURCE_DIR} \( -name '*.h' -o -name '*.cpp' \) \
			-not -path '${BUILD_DIR}/*')
