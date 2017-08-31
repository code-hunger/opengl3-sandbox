#ifndef CONFIG_FACTORY_H_EQA17KCO
#define CONFIG_FACTORY_H_EQA17KCO

struct ConfigFactory
{
	struct Data;
	const Data* const data;

	ConfigFactory(int argc, const char** argv);

	ConfigFactory(ConfigFactory&&) = default;

	ConfigFactory(const ConfigFactory&) = delete;
	void operator=(const ConfigFactory&) = delete;

	template <typename T> T produce() const { return {}; }

	template <typename T> void process(T&) const {}

	~ConfigFactory();
};

#endif /* end of include guard: CONFIG_FACTORY_H_EQA17KCO */
