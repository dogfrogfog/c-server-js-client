import React from "react";

export interface PikachuCardProps {
  id: number;
  name: string;
  type: string[];
  stats: {
    hp: number;
    attack: number;
    defense: number;
    speed: number;
  };
  abilities: string[];
  image: string;
}

export const PikachuCard: React.FC<PikachuCardProps> = ({
  //   id,
  name,
  type,
  stats,
  abilities,
  image,
}) => {
  return (
    <div className="bg-yellow-300 p-4 rounded-lg shadow-lg transform transition-transform duration-300 hover:scale-105">
      <img src={image} alt={name} className="w-32 h-32 mx-auto" />
      <h2 className="text-2xl font-bold text-center text-gray-800">{name}</h2>
      <p className="text-center text-gray-600">Type: {type.join(", ")}</p>
      <div className="mt-2">
        <h3 className="font-semibold">Stats:</h3>
        <p>HP: {stats.hp}</p>
        <p>Attack: {stats.attack}</p>
        <p>Defense: {stats.defense}</p>
        <p>Speed: {stats.speed}</p>
      </div>
      <div className="mt-2">
        <h3 className="font-semibold">Abilities:</h3>
        <ul className="list-disc list-inside">
          {abilities.map((ability, index) => (
            <li key={index}>{ability}</li>
          ))}
        </ul>
      </div>
    </div>
  );
};

export default PikachuCard;
