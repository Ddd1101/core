/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
#include "../../../ASCOfficePPTFile/PPTFormatLib/Records/RoundTripThemeAtom.h"
#include "Structures.h"
#include "Layout.h"

namespace PPT_FORMAT
{
	enum _typeMaster
	{
		typeMaster,
		typeNotesMaster,
		typeHandoutMaster

	};
	
	const long g_ThemeTextStylesCount	= 4;

	class CTheme
	{
	public:
		_typeMaster						m_eType;
		std::map<_UINT64, LONG>			m_mapGeomToLayout;// типовые шаблоны
		std::map<_UINT32, LONG>			m_mapTitleLayout; // заголовочные шаблоны
		
		std::multimap<int,CElementPtr>	m_mapPlaceholders;

		std::vector<CColor>				m_arColorScheme;
		std::vector<CFontProperty>		m_arFonts;

		std::vector<CBrush>				m_arBrushes;
		std::vector<CPen>				m_arPens;

		CFontProperties					majorFont;
		CFontProperties					minorFont;

		std::vector<CEffects>			m_arEffects;
		CTextStyles						m_pStyles[g_ThemeTextStylesCount];
		std::vector<CLayoutPtr>			m_arLayouts;

		bool							m_bIsBackground;
		CBrush							m_oBackground;

		std::vector<CElementPtr>		m_arElements;

		std::wstring					m_sThemeName;
		
		std::vector<std::vector<CColor>>m_arExtraColorScheme;

        std::vector<CRecordRoundTripThemeAtom*> m_arrZipXml;
	
		bool							m_bHasDate;
		bool							m_bHasSlideNumber;
		bool							m_bHasFooter;

		int								m_nFormatDate;//1- current, 2 - user

		vector_string					m_PlaceholdersReplaceString[3]; //0-dates, 1 - headers, 2 - footers

        std::vector<CBulletAutoNum>     m_arBullet;
        bool m_wasBulletStart;
        unsigned m_bulletPos;
//------------------------------------------------------------------------------------
		CTheme(_typeMaster type = typeMaster) : m_arColorScheme(), m_arFonts(), m_arBrushes(),
            m_arPens(), m_arEffects(), m_arLayouts(), m_eType(type), m_wasBulletStart(false),
            m_bulletPos(0)
		{
			Clear();
		}
		void Clear()
		{
			m_arElements.clear();
			m_arLayouts.clear();
			m_mapTitleLayout.clear();
			m_mapGeomToLayout.clear();
			m_mapPlaceholders.clear();
            m_arrZipXml.clear();

			m_sThemeName		= L"Default";
			
			m_bHasDate			= false;
			m_bHasSlideNumber	= false;
			m_bHasFooter		= false;
			m_nFormatDate		= 1;
		
			for (int i = 0 ; i < 3 ; i++) 
				m_PlaceholdersReplaceString[i].clear();
		}

		virtual ~CTheme()
		{
		}
		ODRAW::CColor GetColor(const LONG& lIndexScheme)
		{
			if (lIndexScheme < (LONG)m_arColorScheme.size())
			{
				return m_arColorScheme[lIndexScheme];
			}
			return ODRAW::CColor();
		}

		void CalculateStyles()
		{
			LONG lCountColors = (LONG)m_arColorScheme.size();

			for (int pos = 0; pos < g_ThemeTextStylesCount; ++pos)
			{
				PPT_FORMAT::CTextStyles& oStyle = m_pStyles[pos];

				size_t nLevels = 10;
				for (size_t i = 0; i < nLevels; ++i)
				{
					if (!oStyle.m_pLevels[i].is_init())
						continue;

					CTextStyleLevel* pLevel = oStyle.m_pLevels[i].operator ->();

					if (pLevel->m_oPFRun.bulletColor.is_init())
					{
						CColor& oColor = pLevel->m_oPFRun.bulletColor.get();
						if ((0 <= oColor.m_lSchemeIndex) && (oColor.m_lSchemeIndex < lCountColors))
						{
							oColor.R	= m_arColorScheme[oColor.m_lSchemeIndex].R;
							oColor.G	= m_arColorScheme[oColor.m_lSchemeIndex].G;
							oColor.B	= m_arColorScheme[oColor.m_lSchemeIndex].B;
							oColor.A	= m_arColorScheme[oColor.m_lSchemeIndex].A;
						}
					}
					if (pLevel->m_oCFRun.Color.is_init())
					{
						CColor& oColor = pLevel->m_oCFRun.Color.get();
						if ((0 <= oColor.m_lSchemeIndex) && (oColor.m_lSchemeIndex < lCountColors))
						{
							oColor.R	= m_arColorScheme[oColor.m_lSchemeIndex].R;
							oColor.G	= m_arColorScheme[oColor.m_lSchemeIndex].G;
							oColor.B	= m_arColorScheme[oColor.m_lSchemeIndex].B;
							oColor.A	= m_arColorScheme[oColor.m_lSchemeIndex].A;
						}
					}
					if (pLevel->m_oPFRun.bulletFontRef.IsInit())
					{
						WORD lFontIndex = pLevel->m_oPFRun.bulletFontRef.get();
						if (lFontIndex < (WORD)m_arFonts.size())
						{
							if (!pLevel->m_oPFRun.bulletFontProperties.is_init())
								pLevel->m_oPFRun.bulletFontProperties = new CFontProperty(m_arFonts[lFontIndex]);
						}
					}
					if (pLevel->m_oCFRun.fontRef.IsInit())
					{
						WORD lFontIndex = pLevel->m_oCFRun.fontRef.get();
						if (lFontIndex < (WORD)m_arFonts.size())
						{
							if (!pLevel->m_oCFRun.font.font.is_init())
								pLevel->m_oCFRun.font.font = new CFontProperty(m_arFonts[lFontIndex]);
						}
					}
					if (pLevel->m_oCFRun.eaFontRef.IsInit())
					{
						WORD lFontIndex = pLevel->m_oCFRun.eaFontRef.get();
						if (lFontIndex < (WORD)m_arFonts.size())
						{
							if (!pLevel->m_oCFRun.font.ea.is_init())
								pLevel->m_oCFRun.font.ea = new CFontProperty(m_arFonts[lFontIndex]);
						}
					}
					if (pLevel->m_oCFRun.ansiFontRef.IsInit())
					{
						WORD lFontIndex = pLevel->m_oCFRun.ansiFontRef.get();
						if (lFontIndex < (WORD)m_arFonts.size())
						{
							if (!pLevel->m_oCFRun.font.ansi.is_init())
								pLevel->m_oCFRun.font.ansi = new CFontProperty(m_arFonts[lFontIndex]);
						}
					}
					if (pLevel->m_oCFRun.symbolFontRef.IsInit())
					{
						WORD lFontIndex = pLevel->m_oCFRun.symbolFontRef.get();
						if (lFontIndex < (WORD)m_arFonts.size())
						{
							if (!pLevel->m_oCFRun.font.sym.is_init())
								pLevel->m_oCFRun.font.sym = new CFontProperty(m_arFonts[lFontIndex]);
						}
					}
				}
			}
		}

		static void CalculateStyle(CTheme* pTheme, CTextStyles& oStyle)
		{
			LONG lCountColors = (LONG)pTheme->m_arColorScheme.size();

			// пока здесь расчитываются только цвета
			size_t nLevels = 10;
			for (size_t i = 0; i < nLevels; ++i)
			{
				if (!oStyle.m_pLevels[i].is_init())
					continue;

				CTextStyleLevel* pLevel = oStyle.m_pLevels[i].operator ->();

				if (pLevel->m_oPFRun.bulletColor.is_init())
				{
					CColor& oColor = pLevel->m_oPFRun.bulletColor.get();
					if ((0 <= oColor.m_lSchemeIndex) && (oColor.m_lSchemeIndex < lCountColors))
					{
						oColor.R	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].R;
						oColor.G	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].G;
						oColor.B	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].B;
						oColor.A	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].A;
					}
				}
				if (pLevel->m_oCFRun.Color.is_init())
				{
					CColor& oColor = pLevel->m_oCFRun.Color.get();
					if ((0 <= oColor.m_lSchemeIndex) && (oColor.m_lSchemeIndex < lCountColors))
					{
						oColor.R	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].R;
						oColor.G	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].G;
						oColor.B	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].B;
						oColor.A	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].A;
					}
				}
				if (pLevel->m_oCFRun.fontRef.IsInit())
				{
					WORD lFontIndex = pLevel->m_oCFRun.fontRef.get();
					if (lFontIndex < (WORD)pTheme->m_arFonts.size())
					{
						if (!pLevel->m_oCFRun.font.font.is_init())
							pLevel->m_oCFRun.font.font = new CFontProperty(pTheme->m_arFonts[lFontIndex]);
					}
				}
				if (pLevel->m_oCFRun.eaFontRef.IsInit())
				{
					WORD lFontIndex = pLevel->m_oCFRun.eaFontRef.get();
					if (lFontIndex < (WORD)pTheme->m_arFonts.size())
					{
						if (!pLevel->m_oCFRun.font.ea.is_init())
							pLevel->m_oCFRun.font.ea = new CFontProperty(pTheme->m_arFonts[lFontIndex]);
					}
				}
				if (pLevel->m_oCFRun.ansiFontRef.IsInit())
				{
					WORD lFontIndex = pLevel->m_oCFRun.ansiFontRef.get();
					if (lFontIndex < (WORD)pTheme->m_arFonts.size())
					{
						if (!pLevel->m_oCFRun.font.ansi.is_init())
							pLevel->m_oCFRun.font.ansi = new CFontProperty(pTheme->m_arFonts[lFontIndex]);
					}
				}
				if (pLevel->m_oCFRun.symbolFontRef.IsInit())
				{
					WORD lFontIndex = pLevel->m_oCFRun.symbolFontRef.get();
					if (lFontIndex < (WORD)pTheme->m_arFonts.size())
					{
						if (!pLevel->m_oCFRun.font.sym.is_init())
							pLevel->m_oCFRun.font.sym = new CFontProperty(pTheme->m_arFonts[lFontIndex]);
					}
				}
			}
		}
	};
	typedef boost::shared_ptr<CTheme> CThemePtr;

}
