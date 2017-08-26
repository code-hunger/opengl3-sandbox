#ifndef CONFIG_FACTORY_H_EQA17KCO
#define CONFIG_FACTORY_H_EQA17KCO

class ConfigFactory
{
	struct Data;
	const Data* data;

public:
	ConfigFactory(int argc, const char** argv);

	ConfigFactory(ConfigFactory&&) = default;

	ConfigFactory(const ConfigFactory&) = delete;
	void operator=(const ConfigFactory&) = delete;

	template <typename T> T produce() { return {}; }

	template <typename T> void process(T&) {}

	~ConfigFactory();
};

#endif /* end of include guard: CONFIG_FACTORY_H_EQA17KCO */
