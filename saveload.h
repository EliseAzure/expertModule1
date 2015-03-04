#ifndef SAVELOAD
#define SAVELOAD

#include "model.h"

class SaveLoad
{
	public:
		SaveLoad();
		~SaveLoad();
		static void save(QVector<Plot> models, QString filename);
		static void load(QVector<Plot>* models, QString filename);
};

#endif // SAVELOAD

