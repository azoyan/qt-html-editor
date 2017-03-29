#include "Finder.h"
#include <QDebug>
#include "Global.h"

Finder::Finder(const QString& text, const QString& target)
: mText(text)
{
  if (!target.isEmpty()) find(target);
}

void Finder::setText(const QString& text) {
  mText = text;
}

int Finder::find(const QString& target) {
  int index = 0;
  while ((index = mText.indexOf(target, index)) != -1) mSymbolPositions << index++;
  return mSymbolPositions.isEmpty() ? Search::NotFound : mSymbolPositions.back();
}

bool Finder::hasFounded() const {
  return !mSymbolPositions.isEmpty();
}

Finder::SymbolPositions Finder::symbolPositions() const {
  return mSymbolPositions;
}
