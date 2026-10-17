#include <gtest/gtest.h>

#include <Model/Player.h>
using namespace lhs::model;


namespace
{
	Painting MakePainting(std::string title, std::string painter)
	{
		return { .path = {}, .title = std::move(title), .painter = std::move(painter) };
	}
}

TEST(PlayerTest, StartsWithHundredGoldAndNoPaintings)
{
	const Player player{ 3, "Andrew" };

	EXPECT_EQ(player.GetId(), 3u);
	EXPECT_EQ(player.GetName(), "Andrew");
	EXPECT_EQ(player.GetGold(), 100u);
	EXPECT_TRUE(player.GetPaintings().empty());
}

TEST(PlayerTest, AddsAndSubtractsGold)
{
	Player player{ 0, "You" };

	player.AddGold(25);
	player.SubtractGold(40);

	EXPECT_EQ(player.GetGold(), 85u);
}

TEST(PlayerTest, GetPaintingReturnsAndRemovesIt)
{
	Player player{ 0, "You" };
	player.SetPaintings({ MakePainting("A", "Goya"), MakePainting("B", "Hokusai") });

	const auto painting = player.GetPainting(0);

	EXPECT_EQ(painting.title, "A");
	ASSERT_EQ(player.GetPaintings().size(), 1u);
	EXPECT_EQ(player.GetPaintings().front().title, "B");
}

TEST(PlayerTest, RemovePaintingMatchesByTitle)
{
	Player player{ 0, "You" };
	player.SetPaintings({ MakePainting("A", "Goya"), MakePainting("B", "Goya") });

	player.RemovePainting(MakePainting("A", "someone else"));

	ASSERT_EQ(player.GetPaintings().size(), 1u);
	EXPECT_EQ(player.GetPaintings().front().title, "B");
}

TEST(PlayerTest, GetInformationsCountsPaintingsForEveryPainter)
{
	Player player{ 0, "You" };
	player.SetPaintings({ MakePainting("A", "Goya"), MakePainting("B", "Goya"), MakePainting("C", "Hokusai") });

	const auto information = player.GetInformations({ "Goya", "Hokusai", "Kandinsky" });

	const std::map<std::string, uint32_t> expected{ { "Goya", 2 }, { "Hokusai", 1 }, { "Kandinsky", 0 } };
	EXPECT_EQ(information, expected);
}

TEST(PlayerTest, SellAllPaysReputationPerPaintingAndEmptiesTheGallery)
{
	Player player{ 0, "You" };
	player.SetPaintings({ MakePainting("A", "Goya"), MakePainting("B", "Goya"), MakePainting("C", "Hokusai") });

	player.SellAll({ { "Goya", 10 }, { "Hokusai", 3 } });

	EXPECT_EQ(player.GetGold(), 100u + 10 + 10 + 3);
	EXPECT_TRUE(player.GetPaintings().empty());
}
