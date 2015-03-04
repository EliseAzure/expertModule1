#ifndef SAVELOAD
#define SAVELOAD

#include "model.h"

class SaveLoad
{
	public:
		SaveLoad();
		~SaveLoad();
		static void save(QVector<Model> models, QString filename);
		static void load(QVector<Model>* models, QString filename);
};

#endif // SAVELOAD

