#include "depch.h"
#include "NativeScriptComponent.h"

namespace OpenGLEngine
{
	NativeScriptComponent::NativeScriptComponent()
	{
		hinstance = LoadLibraryA("ProjectSolution/bin/Release-windows-x86_64/ProjectSolution/ProjectSolution.dll");
		if (hinstance)
		{
			//fun_script_creator = (_get_script_creator)GetProcAddress(hinstance, "get_script_creator");
			//fun_script_names = (_get_script_names)GetProcAddress(hinstance, "get_script_names");

			/*// Get the SafeArray of script names
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

			//std::cout << stringVector.at(0).c_str() << std::endl;

			const wchar_t* wideStr = stringVector.at(0).c_str();
			size_t bufferSize = wcslen(wideStr) + 1;  // +1 pour le caractère nul de fin
			char* narrowStr = new char[bufferSize];

			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, narrowStr, bufferSize, wideStr, _TRUNCATE);

			// Imprime la chaîne C étroite
			std::cout << "Contenu de la premiere chaine (C etroite) : " << narrowStr << std::endl;

			wideStr = stringVector.at(1).c_str();
			bufferSize = wcslen(wideStr) + 1;
			narrowStr = new char[bufferSize];
			convertedChars = 0;
			wcstombs_s(&convertedChars, narrowStr, bufferSize, wideStr, _TRUNCATE);

			std::cout << "Contenu de la deuxieme chaine (C etroite) : " << narrowStr << std::endl;*/

			// N'oubliez pas de libérer la mémoire allouée
			//delete[] narrowStr;

			/*script_creator script;
			script = fun_script_creator(string_hash()(result));

			if (script)
			{
				std::cout << "Script validate" << std::endl;
			}*/

			//FreeLibrary(hinstance);
		}
	}

	ScriptableEntity* NativeScriptComponent::InstantiateScript()
	{
		return m_Script.get();
	}

	void NativeScriptComponent::Bind()
	{
		//if (m_ScriptName.empty())
			//return;

		if (hinstance)
		{
			std::cout << "Get entity" << std::endl;
			fun_script_creator = (_get_script_creator)GetProcAddress(hinstance, "get_script_creator");
			fun_script_names = (_get_script_names)GetProcAddress(hinstance, "get_script_names");
			script_creator var_script_creator = fun_script_creator(string_hash()("Player"));

			if (var_script_creator)
			{
				std::cout << "Script validate" << std::endl;

				m_Script = var_script_creator();

				//Instance = var_script_creator().get();

				//entity->OnCreate();
			}
		}
	}
}