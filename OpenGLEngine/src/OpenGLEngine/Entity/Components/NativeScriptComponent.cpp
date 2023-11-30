#include "depch.h"
#include "NativeScriptComponent.h"

namespace OpenGLEngine
{
	NativeScriptComponent::NativeScriptComponent()
	{
		hinstance = LoadLibraryA("ProjectSolution/bin/Release-windows-x86_64/ProjectSolution/ProjectSolution.dll");
		if (hinstance)
		{
			fun_script_names = (_get_script_names)GetProcAddress(hinstance, "get_script_names");

			// Get the SafeArray of script names
			LPSAFEARRAY scriptNames = fun_script_names();

			if (scriptNames == nullptr) {
				std::cerr << "LPSAFEARRAY is null." << std::endl;
			}

			// Obtient les dimensions du tableau
			UINT dimCount = SafeArrayGetDim(scriptNames);

			// Vérifie que la dimension est égale à 1 (tableau unidimensionnel)
			if (dimCount != 1) {
				std::cerr << "LPSAFEARRAY is not one-dimensional." << std::endl;
			}

			// Obtient la taille du tableau
			LONG lBound, uBound;
			SafeArrayGetLBound(scriptNames, 1, &lBound);
			SafeArrayGetUBound(scriptNames, 1, &uBound);

			// Initialise le vecteur pour contenir les chaînes
			std::vector<std::wstring> stringVector;

			// Parcourt le tableau et ajoute chaque élément au vecteur
			for (LONG i = lBound; i <= uBound; ++i) {
				BSTR bstr;
				SafeArrayGetElement(scriptNames, &i, &bstr);

				// Convertit BSTR en std::wstring
				std::wstring str(bstr);

				// Ajoute la chaîne au vecteur
				stringVector.push_back(str);

				// Libère la mémoire allouée pour le BSTR
				SysFreeString(bstr);
			}

			for (int i = 0; i < stringVector.size(); i++)
			{
				const wchar_t* wideStr = stringVector.at(i).c_str();
				size_t bufferSize = wcslen(wideStr) + 1;  // +1 pour le caractère nul de fin
				char* narrowStr = new char[bufferSize];

				size_t convertedChars = 0;
				wcstombs_s(&convertedChars, narrowStr, bufferSize, wideStr, _TRUNCATE);

				m_LoadedScriptNames.push_back(narrowStr);

				delete[] narrowStr;
			}

			//FreeLibrary(hinstance);
		}
	}

	ScriptableEntity* NativeScriptComponent::InstantiateScript()
	{
		return m_Script.get();
	}

	void NativeScriptComponent::Bind()
	{
		if (m_ScriptName.empty())
			return;

		m_Script = nullptr;

		if (hinstance)
		{
			fun_script_creator = (_get_script_creator)GetProcAddress(hinstance, "get_script_creator");
			fun_script_names = (_get_script_names)GetProcAddress(hinstance, "get_script_names");
			script_creator var_script_creator = fun_script_creator(string_hash()(m_ScriptName.c_str()));

			if (var_script_creator)
			{
				m_Script = var_script_creator();
			}
		}
	}
}