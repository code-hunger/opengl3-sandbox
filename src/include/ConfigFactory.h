#ifndef CONFIG_FACTORY_H_EQA17KCO
#define CONFIG_FACTORY_H_EQA17KCO

#include "ScreenManager.h"
#include "tclap/CmdLine.h"

struct ConfigFactory
{
	ushort maze_id, max_lines;
	bool no_join_lines;

public:
	ConfigFactory(int argc, char** argv)
	{
		using namespace TCLAP;

		CmdLine cmd("ScreenManager title", ' ', "0.1");
		UnlabeledValueArg<ushort> maze_id("maze_id", "Maze id to load", false,
		                                  1, "number", cmd);
		ValueArg<ushort> max_lines("m", "max_lines", "Max ways to be added",
		                           false, 0, "count", cmd);
		SwitchArg no_join_lines("n", "nojoin", "Don't join intersecting lines",
		                        cmd, true),
		    print_fps("f", "print-fps", "prints fps", cmd);

		cmd.parse(argc, argv);

		this->maze_id = maze_id.getValue();
		this->no_join_lines = no_join_lines.getValue();
		this->max_lines = max_lines.getValue();
	}

	template <typename T> T produce() { return {}; };
};

template <> ScreenManager ConfigFactory::produce()
{
	return {maze_id, no_join_lines, max_lines};
}

#endif /* end of include guard: CONFIG_FACTORY_H_EQA17KCO */
