include(FetchContent)

FetchContent_Declare(
	minhook
	GIT_REPOSITORY https://github.com/danielga/minhook.git
	GIT_TAG        e2a49551a5c8b372cb962720997dc5c43df6b4b9
	GIT_PROGRESS TRUE
)
message("MinHook")
FetchContent_MakeAvailable(minhook)