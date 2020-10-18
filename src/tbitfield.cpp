// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"


TBitField::TBitField(int len)
{
	if (len < 0) throw len;
	BitLen = len;
	MemLen = floor(len / 32) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	if (MemLen != bf.MemLen) {
		MemLen = bf.MemLen;
		delete [] pMem;
		pMem = new TELEM[MemLen];
	}
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 ) throw n;
	return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0) throw n;
	TELEM one = 1, tmp = (n % 32);
	return (one << tmp);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n > BitLen) throw n;
	int i = GetMemIndex(n);
	TELEM m = GetMemMask(n);
	pMem[i] = pMem[i] | m;

}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n > BitLen) throw n;
	int i = GetMemIndex(n);
	TELEM m = GetMemMask(n);
		pMem[i] &= ~m;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n > BitLen) throw n;
	int i = GetMemIndex(n);
	TELEM m = GetMemMask(n);
    return pMem[i] & m;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (MemLen != bf.MemLen) {
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return false;
	}
	for (int i = 0; i < MemLen-1; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return false;
		}
	}
	for (int i = (MemLen - 1) * 32; i < BitLen; i++) {
		if (GetBit(i) != bf.GetBit(i))
			return false;
	}
  return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this==(bf));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(BitLen >= bf.BitLen ? BitLen : bf.BitLen);
	if (BitLen >= bf.BitLen) {
		for (int i = 0; i < bf.BitLen; i++) {
			if (bf.GetBit(i))
				res.SetBit(i);
			else {
				res.ClrBit(i);
			}
		}
		for (int i = 0; i < MemLen; i++) {
			res.pMem[i] |= pMem[i];
		}
	}
	else {
		for (int i = 0; i < BitLen; i++) {
			if (GetBit(i))
				res.SetBit(i);
		}
		for (int i = 0; i < bf.MemLen; i++) {
			res.pMem[i] |= bf.pMem[i];
		}
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(BitLen < bf.BitLen ? MemLen : bf.MemLen);
	if (BitLen < bf.BitLen) {
		for (int i = 0; i < BitLen; i++) {
			res.pMem[i] = pMem[i] | bf.pMem[i];
		}
	}
	else {
		for (int i = 0; i < bf.BitLen; i++) {
			res.pMem[i] = pMem[i] | bf.pMem[i];
		}
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(*this);
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = ~pMem[i];
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char x;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> x;

		if (x == '0')
			bf.ClrBit(i);
		else if (x == '1')
			bf.SetBit(i);
		else
			break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i))
			ostr << '1';
		else
			ostr << '0';
	}
	return ostr;
}
