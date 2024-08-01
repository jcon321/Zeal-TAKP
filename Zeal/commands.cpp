#include "commands.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
#include <cctype>
#include "string_util.h"


void ChatCommands::print_commands()
{
	std::stringstream ss;
	ss << "List of commands" << std::endl;
	ss << "-----------------------------------------------------" << std::endl;
	for (auto& [name, c] : CommandFunctions)
	{
		ss << name;
		if (c.aliases.size() > 0)
			ss << " [";
		for (auto it = c.aliases.begin(); it != c.aliases.end(); ++it) {
			auto& a = *it;
			ss << a;
			if (std::next(it) != c.aliases.end()) {
				ss << ", ";
			}
		}
		if (c.aliases.size() > 0)
			ss << "]";

		ss << " " << c.description;
		ss << std::endl;
	}
	Zeal::EqGame::print_chat(ss.str());
}

void __fastcall InterpretCommand(int c, int unused, int player, char* cmd)
{
	ZealService* zeal = ZealService::get_instance();
	std::string str_cmd = Zeal::String::trim_and_reduce_spaces(cmd);
	if (str_cmd.length() == 0)
		return;
	if (str_cmd.length()>0 && str_cmd.front() != '/')
		str_cmd = "/" + str_cmd;
	std::vector<std::string> args = Zeal::String::split(str_cmd," ");
	const std::string& cmd_name = args.front();
	if (!args.empty() && !cmd_name.empty() && Zeal::EqGame::is_in_game())
	{
		bool cmd_handled = false;
		auto& command_functions = zeal->commands_hook->CommandFunctions;
		if (command_functions.count(cmd_name))
		{
			cmd_handled = command_functions[args[0]].callback(args);
		} 
		else
		{
			//attempt to find and call the function via an alias
			for (const auto& command_pair : command_functions) {
				const auto& command = command_pair.second;
				if (command.callback && !command.aliases.empty()) {
					for (const auto alias : command.aliases) {
						if (!alias.empty() && alias == cmd_name) {
							cmd_handled = command.callback(args);
							break;
						}
					}
				}
			}
		}
		if (cmd_handled) {
			return;
		}
	}
	zeal->hooks->hook_map["commands"]->original(InterpretCommand)(c, unused, player, cmd);
}

void ChatCommands::Add(std::string cmd, std::vector<std::string>aliases, std::string description, std::function<bool(std::vector<std::string>&args)> callback)
{
	CommandFunctions[cmd] = ZealCommand(aliases, description, callback);
}

ChatCommands::~ChatCommands()
{

}
ChatCommands::ChatCommands(ZealService* zeal)
{
	Add("/zeal", { "/zea" }, "Help and version information.",
		[this](std::vector<std::string>& args) {
			if (args.size() == 1)
			{
				Zeal::EqGame::print_chat("Available args: version, help"); //leave room for more args on this command for later
				return true;
			}
			if (args.size() > 1 && Zeal::String::compare_insensitive(args[1], "version"))
			{
				std::stringstream ss;
				ss << "Zeal version: " << ZEAL_VERSION << std::endl;
				Zeal::EqGame::print_chat(ss.str());
				return true;
			}
			if (args.size() > 1 && Zeal::String::compare_insensitive(args[1], "help"))
			{
				print_commands();
				return true;
			}
			return false;
		});
	zeal->hooks->Add("commands", Zeal::EqGame::EqGameInternal::fn_interpretcmd, InterpretCommand, hook_type_detour);
}

