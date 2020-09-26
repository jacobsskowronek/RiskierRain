#include <Windows.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "Hooks.h"
#include "Cheats.h"


const LPCWSTR GameTitle = L"Risk of Rain 2";


// Enumerate tagged GameObjects to find players
// m_CachedPtr contains address of native function


//const char* sig = "\x7B\x94\x30\x00\x04\x37\x13\x02\x28\xAD\x2E\x00\x06\x2C\x0B\x02\x7B\xE3\x28\x00\x04\x6F\xF0\x0B\x00\x06"; // +16 : + 5
//const char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxxx";


/*
void _cdecl AssemblyEnumerator(void* domain, std::vector<UINT64>* v)
{
	v->push_back((UINT_PTR)domain);
}

HMODULE GetMonoModule()
{
	HMODULE hMono = NULL;

	HANDLE ths = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (ths != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 me;
		me.dwSize = sizeof(me);

		if (Module32First(ths, &me))
		{
			do
			{
				if (GetProcAddress(me.hModule, "mono_thread_attach"))
				{
					hMono = me.hModule;
					break;
				}

				if (GetProcAddress(me.hModule, ("il2cpp_thread_attach")))
				{
					hMono = me.hModule;
					break;
				}

			} while (Module32Next(ths, &me));

		}
		CloseHandle(ths);
	}

	return hMono;
}








void* image = 0;

uint32_t GetOffset(std::string classname, std::string fieldname, bool fromfieldtype = false)
{
	if (!image)
	{
		std::vector<UINT64> v;

		mono_assembly_foreach((GFunc)AssemblyEnumerator, &v);

		for (auto i = 0; i < v.size(); i++)
		{
			auto assembly = v[i];

			if (!assembly)
				continue;

			image = mono_assembly_get_image((void*)assembly);

			if (!image)
				continue;

			if (strcmp(mono_image_get_name(image), "Assembly-CSharp") == 0)
				break;
		}
	}

	void* monoclass = 0;

	void* tdef = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);


	if (tdef)
	{
		int tdefcount = mono_table_info_get_rows(tdef);

		for (auto i = 0; i < tdefcount; i++)
		{
			monoclass = mono_class_get(image, MONO_TOKEN_TYPE_DEF | i + 1);
			if (monoclass != NULL)
			{
				char* name = mono_class_get_name(monoclass);
				

				if (strcmp(name, classname.c_str()) == 0) {
					break;
				}

			}
		}
	}
	void* iter = NULL;
	void* field;

	do
	{
		field = mono_class_get_fields(monoclass, &iter);
		if (field)
		{
			char* name;

			if (fromfieldtype)
			{
				//fieldname has to include namespace

				auto fieldtype = mono_field_get_type(field);
				name = mono_type_get_name(fieldtype);
			}
			else
				name = mono_field_get_name(field);

			if (strcmp(name, fieldname.c_str()) == 0)
				break;

		}
	} while (field);

	if (!field)
		return 0;

	return mono_field_get_offset(field);
}


void* GetMethod(std::string classname, std::string methodname, std::string name_space)
{

	if (!image)
	{
		std::vector<UINT64> v;

		mono_assembly_foreach((GFunc)AssemblyEnumerator, &v);

		for (auto i = 0; i < v.size(); i++)
		{
			auto assembly = v[i];

			if (!assembly)
				continue;

			image = mono_assembly_get_image((void*)assembly);

			if (!image)
				continue;

			if (strcmp(mono_image_get_name(image), "Assembly-CSharp") == 0)
				break;
		}
	}

	void* monoclass = 0;

	void* tdef = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

	if (tdef)
	{
		int tdefcount = mono_table_info_get_rows(tdef);

		for (auto i = 0; i < tdefcount; i++)
		{
			monoclass = mono_class_get(image, MONO_TOKEN_TYPE_DEF | i + 1);
			if (monoclass != NULL)
			{
				char* name = mono_class_get_name(monoclass);


				if (strcmp(name, classname.c_str()) == 0) {
					std::cout << "Mono class: " << std::hex << monoclass << std::endl;
					std::cout << "Mono classname: " << name << std::endl;

					char* classnamespace;
					classnamespace = mono_class_get_namespace(monoclass);

					if (strcmp(classnamespace, name_space.c_str()) == 0) {
						std::cout << "Namespace: " << classnamespace << std::endl;
						break;
					}

					

				
					
				}

			}
		}
	}
	void* iter = NULL;
	void* method = 0;

	do
	{
		method = mono_class_get_methods(monoclass, &iter);
		if (method)
		{
			char* name;

			
			name = mono_method_get_name(method);

			if (strcmp(name, methodname.c_str()) == 0) {
				std::cout << "Method name: " << name << std::endl;
				std::cout << "Method: " << method << std::endl;

				break;

			}

		}
	} while (method);

	if (!method)
		return 0;

	return method;
}
*/

int main()
{

	//AllocConsole();
	//freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);





	HWND hWindow = FindWindowW(NULL, GameTitle);

	HMODULE hModule = GetModuleHandle(NULL);

	Initialize_Hooks(hModule, hWindow);


	/*
	auto hMono = GetMonoModule();

	mono_assembly_foreach = (MONO_ASSEMBLY_FOREACH)GetProcAddress(hMono, "mono_assembly_foreach");
	mono_assembly_get_image = (MONO_ASSEMBLY_GET_IMAGE)GetProcAddress(hMono, "mono_assembly_get_image");
	mono_image_get_name = (MONO_IMAGE_GET_NAME)GetProcAddress(hMono, "mono_image_get_name");
	mono_image_get_table_info = (MONO_IMAGE_GET_TABLE_INFO)GetProcAddress(hMono, "mono_image_get_table_info");
	mono_table_info_get_rows = (MONO_TABLE_INFO_GET_ROWS)GetProcAddress(hMono, "mono_table_info_get_rows");
	mono_class_get = (MONO_CLASS_GET)GetProcAddress(hMono, "mono_class_get");
	mono_class_get_name = (MONO_CLASS_GET_NAME)GetProcAddress(hMono, "mono_class_get_name");
	mono_class_get_fields = (MONO_CLASS_GET_FIELDS)GetProcAddress(hMono, "mono_class_get_fields");
	mono_field_get_name = (MONO_FIELD_GET_NAME)GetProcAddress(hMono, "mono_field_get_name");
	mono_field_get_offset = (MONO_FIELD_GET_OFFSET)GetProcAddress(hMono, "mono_field_get_offset");
	mono_field_get_type = (MONO_FIELD_GET_TYPE)GetProcAddress(hMono, "mono_field_get_type");
	mono_type_get_name = (MONO_TYPE_GET_NAME)GetProcAddress(hMono, "mono_type_get_name");
	mono_class_get_methods = (MONO_CLASS_GET_METHODS)GetProcAddress(hMono, "mono_class_get_methods");
	mono_method_get_name = (MONO_METHOD_GET_NAME)GetProcAddress(hMono, "mono_method_get_name");
	mono_class_get_parent = (MONO_CLASS_GET_PARENT)GetProcAddress(hMono, "mono_class_get_parent");
	mono_class_get_namespace = (MONO_CLASS_GET_NAMESPACE)GetProcAddress(hMono, "mono_class_get_namespace");
	mono_compile_method = (MONO_COMPILE_METHOD)GetProcAddress(hMono, "mono_compile_method");
	mono_class_get_method_from_name = (MONO_CLASS_GET_METHOD_FROM_NAME)GetProcAddress(hMono, "mono_class_get_method_from_name");
	mono_class_is_generic = (MONO_CLASS_IS_GENERIC)GetProcAddress(hMono, "mono_class_is_generic");
	mono_method_get_class = (MONO_METHOD_GET_CLASS)GetProcAddress(hMono, "mono_method_get_class");

	*/
	std::cout << "Never called" << std::endl;

		


	//std::this_thread::sleep_for(std::chrono::milliseconds(200));


}



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
		break;
	}

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}