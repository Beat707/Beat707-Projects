/*

  A set of custom made large numbers for a 16xPos2 LCD using the 
  LiquidCrystal librabry. Made by Michael Pilcher 2/9/2010
  
*/

// ======================================================================================= //
// the 8 arrays that form each segment of the custom numbers
byte LT[8] = 
{
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte UB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte RT[8] =
{
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte LL[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111
};
byte LB[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte LR[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
};
byte UMB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte LMB[8] =
{
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};

// =================================================== //

void setupBigFont()
{
  // assignes each segment a write number
  lcd.createChar(8,LT);
  lcd.createChar(1,UB);
  lcd.createChar(2,RT);
  lcd.createChar(3,LL);
  lcd.createChar(4,LB);
  lcd.createChar(5,LR);
  lcd.createChar(6,UMB);
  lcd.createChar(7,LMB);
}

/*void custom0O()
{ // uses segments to build the number 0
  lcd.setCursor(xPos, 0); 
  lcd.write(8);  
  lcd.write(1); 
  lcd.write(2);
  lcd.setCursor(xPos, 1); 
  lcd.write(3);  
  lcd.write(4);  
  lcd.write(5);
}

void custom1()
{
  lcd.setCursor(xPos,0);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(xPos+1,1);
  lcd.write(255);
}

void custom2()
{
  lcd.setCursor(xPos,0);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(xPos, 1);
  lcd.write(3);
  lcd.write(7);
  lcd.write(7);
}

void custom3()
{
  lcd.setCursor(xPos,0);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(xPos, 1);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5); 
}

void custom4()
{
  lcd.setCursor(xPos,0);
  lcd.write(3);
  lcd.write(4);
  lcd.write(2);
  lcd.setCursor(xPos+2, 1);
  lcd.write(255);
}

void custom5()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(xPos, 1);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5);
}

void custom6()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(xPos, 1);
  lcd.write(3);
  lcd.write(7);
  lcd.write(5);
}

void custom7()
{
  lcd.setCursor(xPos,0);
  lcd.write(1);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(xPos+1, 1);
  lcd.write(8);
}

void custom8()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(xPos, 1);
  lcd.write(3);
  lcd.write(7);
  lcd.write(5);
}

void custom9()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(xPos+2, 1);
  lcd.write(255);
}*/

void customA()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(xPos, 1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(255);
}

void customB()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(6);
  lcd.write(5);
  lcd.setCursor(xPos, 1);
  lcd.write(255);
  lcd.write(7);
  lcd.write(2);
}

void customC()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(1);
  lcd.write(1);
  lcd.setCursor(xPos,1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(4);
}

void customD()
{
  lcd.setCursor(xPos, 0); 
  lcd.write(255);  
  lcd.write(1); 
  lcd.write(2);
  lcd.setCursor(xPos, 1); 
  lcd.write(255);  
  lcd.write(4);  
  lcd.write(5);
}

/*void customE()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(xPos, 1);
  lcd.write(255);
  lcd.write(7);
  lcd.write(7); 
}

void customF()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(xPos, 1);
  lcd.write(255);
}

void customG()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(1);
  lcd.write(1);
  lcd.setCursor(xPos,1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(2);
}

void customH()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(4);
  lcd.write(255);
  lcd.setCursor(xPos, 1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(255); 
}

void customI()
{
  lcd.setCursor(xPos,0);
  lcd.write(1);
  lcd.write(255);
  lcd.write(1);
  lcd.setCursor(xPos,1);
  lcd.write(4);
  lcd.write(255);
  lcd.write(4);
}

void customJ()
{
  lcd.setCursor(xPos+2,0);
  lcd.write(255);
  lcd.setCursor(xPos,1);
  lcd.write(4);
  lcd.write(4);
  lcd.write(5);
}

void customK()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(xPos,1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(2); 
}

void customL()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.setCursor(xPos,1);
  lcd.write(255);
  lcd.write(4);
  lcd.write(4);
}

void customM()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(3);
  lcd.write(5);
  lcd.write(2);
  lcd.setCursor(xPos,1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(254);
  lcd.write(255);
}

void customN()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(2);
  lcd.write(254);
  lcd.write(255);
  lcd.setCursor(xPos,1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(3);
  lcd.write(5);
}

void customP()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(xPos, 1);
  lcd.write(255);
}

void customQ()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(xPos, 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(255);
  lcd.write(4);
}

void customR()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(xPos,1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(2); 
}

void customS()
{
  lcd.setCursor(xPos,0);
  lcd.write(8);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(xPos, 1);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5);
}

void customT()
{
  lcd.setCursor(xPos,0);
  lcd.write(1);
  lcd.write(255);
  lcd.write(1);
  lcd.setCursor(xPos,1);
  lcd.write(254);
  lcd.write(255);
}

void customU()
{
  lcd.setCursor(xPos, 0); 
  lcd.write(255);  
  lcd.write(254); 
  lcd.write(255);
  lcd.setCursor(xPos, 1); 
  lcd.write(3);  
  lcd.write(4);  
  lcd.write(5);
}

void customV()
{
  lcd.setCursor(xPos, 0); 
  lcd.write(3);  
  lcd.write(254);
  lcd.write(254); 
  lcd.write(5);
  lcd.setCursor(xPos+1, 1); 
  lcd.write(2);  
  lcd.write(8);
}

void customW()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.write(254);
  lcd.write(254);
  lcd.write(255);
  lcd.setCursor(xPos,1);
  lcd.write(3);
  lcd.write(8);
  lcd.write(2);
  lcd.write(5);
}

void customX()
{
  lcd.setCursor(xPos,0);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(xPos,1);
  lcd.write(8);
  lcd.write(254);
  lcd.write(2); 
}

void customY()
{
  lcd.setCursor(xPos,0);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(xPos+1,1);
  lcd.write(255);
}

void customZ()
{
  lcd.setCursor(xPos,0);
  lcd.write(1);
  lcd.write(6);
  lcd.write(5);
  lcd.setCursor(xPos, 1);
  lcd.write(8);
  lcd.write(7);
  lcd.write(4);
}

void customqm()
{
  lcd.setCursor(xPos,0);
  lcd.write(1);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(xPos, 1);
  lcd.write(254);
  lcd.write(7);
}

void customsm()
{
  lcd.setCursor(xPos,0);
  lcd.write(255);
  lcd.setCursor(xPos, 1);
  lcd.write(7);
}*/
