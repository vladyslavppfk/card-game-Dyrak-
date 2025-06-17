#include "GameWindow.h"  
#include "Gameform.h"  

using namespace System;  
using namespace System::Windows::Forms;  

[STAThreadAttribute]  
int main(array<String^>^ args) {  
   Application::EnableVisualStyles();  
   Application::SetCompatibleTextRenderingDefault(false);  

   Dyrakgame::Gameform^ form = gcnew Dyrakgame::Gameform();  
   Application::Run(form);  
}
