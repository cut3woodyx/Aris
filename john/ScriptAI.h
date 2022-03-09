#ifndef _COMPONENT_AI
#define _COMPONENT_AI

#include "IScript.h"
#include <functional>
#include <map>
#include <vector>

namespace FwEngine
{
	constexpr auto STRING_SCRIPT_AI = "AI";

	extern std::vector<std::string> TagsToPush_;

	std::vector<std::string> Tokenize(std::string const& text, std::string const& separator);

	class AIScript;

	class AIBase
	{
		friend class AIScript;
	protected:
		std::string _name;
		AIScript* _compAI;

		ComponentScript* _compScript;

		std::string _variable;
		
	public:
		AIBase(std::string name, AIScript* ai = nullptr, ComponentScript* scr = nullptr, std::string str = "") 
			: _name{ name }, _compAI{ ai }, _compScript{ scr }, _variable{ str }
		{}
		virtual ~AIBase() = default;

		AIBase(const AIBase& rhs) 
			: _name{ rhs._name }, _compAI{ rhs._compAI }, _compScript{ rhs._compScript }, _variable{""} {}
		AIBase& operator=(const AIBase&) {}

		void SetCompAI(AIScript* ci) { _compAI = ci; }
		std::string GetName() { return _name; }

		virtual void Init() = 0;
		virtual void Update(float dt) = 0;
		virtual void Free() = 0;

		virtual void OnCollisionEnter(size_t id) = 0;
		virtual void OnCollisionExit(size_t id) = 0;
		virtual void OnCollisionStay(size_t id) = 0;

		virtual AIBase* Clone(AIScript* = nullptr, ComponentScript* = nullptr, std::string = "") = 0;
	};

	class AIScript : public IScript
	{
		friend class AIBase;

		// Stores all AI behaviours
		typedef std::map<std::string, std::shared_ptr<AIBase>> AIMap;
		static AIMap AllAIBases_;
		std::string _listOfAI;
		std::string _listOfVars;

		// Stores a map of AI behaviours for this instance
		AIMap _aiBases;

		// Current AI
		std::string _curAIName;
		std::shared_ptr<AIBase> _curAI;

	public:
		AIScript(ComponentScript& comp);
		~AIScript();

		void AddAI(std::string aiName, std::string var = "");
		void RemoveAI(std::string aiName);
		void RunAI(std::string name);
		void UpdateAI(std::string aiName, float dt = 0.0f);
		std::shared_ptr<AIBase> GetAI(std::string aiName);

		ComponentScript* GetCompScript();

		template <typename T, class = AIBase>
		static void AddAIBase(std::string AIName)
		{
			AllAIBases_.emplace(AIName, std::make_shared<T>());
		}

		virtual void Init() override;
		virtual void Update(float dt) override;
		virtual void Free() override;
		virtual void Destroy() override;

		virtual void OnCollisionEnter(size_t id) override;
		virtual void OnCollisionExit(size_t id) override;
		virtual void OnCollisionStay(size_t id) override;

		virtual void PushVars() override;
	};

}
#endif

